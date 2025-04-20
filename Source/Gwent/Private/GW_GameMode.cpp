// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "GW_AIController.h"
#include "GW_PlayerController.h"
#include "Card/GW_CardBase.h"
#include "Data/GW_CardDataAsset.h"
#include "Data/GW_PlayerData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Row/GW_Deck.h"
#include "Row/GW_Graveyard.h"
#include "Row/GW_PlayerHand.h"
#include "Row/GW_WeatherRow.h"
#include "UI/GW_HUD.h"


void AGW_GameMode::RegisterRow(AGW_RowBase* NewRow)
{
	if (AGW_UnitRow* NewUnitRow = Cast<AGW_UnitRow>(NewRow))
	{
		if (AGW_WeatherRow* NewWeatherRow = Cast<AGW_WeatherRow>(NewRow))
		{
			this->WeatherRow = NewWeatherRow;
			return;
		}
		
		if (NewRow->GetPlayerID() == EPlayerID::Player1)
		{
			this->RowArrayP1.Add(NewUnitRow);
		}
		else
		{
			this->RowArrayP2.Add(NewUnitRow);
		}
	}
	else if (AGW_Deck* NewDeck = Cast<AGW_Deck>(NewRow))
	{
		if (NewRow->GetPlayerID() == EPlayerID::Player1)
		{
			this->DeckP1 = NewDeck;
		}
		else
		{
			this->DeckP2 = NewDeck;
		}
	}
	else if (AGW_PlayerHand* NewPlayerHand = Cast<AGW_PlayerHand>(NewRow))
	{
		if (NewRow->GetPlayerID() == EPlayerID::Player1)
		{
			this->PlayerHandP1 = NewPlayerHand;
		}
		else
		{
			this->PlayerHandP2 = NewPlayerHand;
		}
	}
	else if (AGW_Graveyard* NewGraveyard = Cast<AGW_Graveyard>(NewRow))
	{
		if (NewRow->GetPlayerID() == EPlayerID::Player1)
		{
			this->GraveyardP1 = NewGraveyard;
		}
		else
		{
			this->GraveyardP2 = NewGraveyard;
		}
	}
}

bool AGW_GameMode::IsMyTurn(const EPlayerID PlayerID) const
{
	if (PlayerID == EPlayerID::Player1 && CurrentGamePhase == EGamePhase::Player1Turn)
	{
		return true;
	}

	if (PlayerID == EPlayerID::Player2 && CurrentGamePhase == EGamePhase::Player2Turn)
	{
		return true;
	}
	
	return false;
}

void AGW_GameMode::EndPlayerTurn(EPlayerID PlayerID)
{
	LastPlayedPlayerID = PlayerID;
	SetGamePhase(EGamePhase::Wait);
}

void AGW_GameMode::PlayerPassedTurn(EPlayerID PlayerID)
{
	if (PlayerID == EPlayerID::Player1)
	{
		Player1Data->SetPassedTurn(true);
		OnAnnouncementMessage.Broadcast("Round passed");
	}
	else
	{
		Player2Data->SetPassedTurn(true);
		OnAnnouncementMessage.Broadcast("Your opponent has passed");
	}

	// check end condition, if both passed turn then end the game/round
	if (Player1Data->IsTurnPassed() && Player2Data->IsTurnPassed())
	{
		GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer,[this]()
		{
			SetGamePhase(EGamePhase::RoundEnd);
		},
	RoundEndWaitDuration,
	false);
	}
	else
	{
		EndPlayerTurn(PlayerID);
	}
}

void AGW_GameMode::SetPlayerHandSize(EPlayerID PlayerID, int32 HandSize)
{
	PlayerID == EPlayerID::Player1 ? Player1Data->SetHandSize(HandSize) : Player2Data->SetHandSize(HandSize);
}

void AGW_GameMode::UpdatePlayerScore(const EPlayerID PlayerID)
{
	if (PlayerID == EPlayerID::Player1)
	{
		Player1Data->SetScore(CalculateScore(RowArrayP1));
	}
	else
	{
		Player2Data->SetScore(CalculateScore(RowArrayP2));
	}
}

int32 AGW_GameMode::CalculateScore(TArray<AGW_UnitRow*> RowArray)
{
	int32 Total = 0;
	for (AGW_UnitRow* Row : RowArray)
	{
		Total += Row->GetTotalPower();
	}
	
	return Total;
}

void AGW_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// define AllRowsArray
	AllRowsArray = RowArrayP1;
	AllRowsArray.Append(RowArrayP2);

	// Create instances of UPlayerData
	Player1Data = UGW_PlayerData::CreateWithPlayerID(this, 1);
	Player2Data = UGW_PlayerData::CreateWithPlayerID(this, 2);
	
	// Get the HUD and bind UI events
	if (AGW_HUD* GameHUD = Cast<AGW_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		Player1Data->OnPlayerDataChanged.AddDynamic(GameHUD, &AGW_HUD::UpdatePlayerData);
		Player2Data->OnPlayerDataChanged.AddDynamic(GameHUD, &AGW_HUD::UpdatePlayerData);
		GameHUD->OnHUDReady.AddUObject(this, &AGW_GameMode::OnHUDReady);
	}
	
	// Initialize RoundScoreData map
	RoundScoreData.Add(Player1Data->Data.PlayerName, TArray({0, 0, 0}));
	RoundScoreData.Add(Player2Data->Data.PlayerName, TArray({0, 0, 0}));


	// start the game with a delay
	GetWorldTimerManager().SetTimer(WaitPhaseTimer, [this]()
	{
		SetGamePhase(EGamePhase::Start);
	}, StartGameWaitDuration, false);
}

void AGW_GameMode::GenerateRandomCardsForDeck()
{
	if (!CardDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No card data available."));
		return;
	}
	
	TArray<FCardData> SelectableCards = CardDataAsset->CardDataArray;

	if (bDebug_FilterCards)
	{
		// remove excluded cards from SelectableCards
		for (int32 i = SelectableCards.Num() - 1; i >= 0; --i)
		{
			if (SelectableCards[i].bDebug_ExcludeFromDeck == true)
			{
				SelectableCards.RemoveAt(i);
			}
		}
	}

	if (SelectableCards.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No suitable card."));
		return;
	}
	
	// give cards to P1's Deck 	// TODO: refactor repeating code
	for (int i = 0; i < InitialDeckSize; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, SelectableCards.Num() - 1);
		const FCardData& SelectedCardData = SelectableCards[RandomIndex];

		// Spawn the card
		AGW_CardBase* Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->InitializeCardData(SelectedCardData);
		Card->Graveyard = GraveyardP1;
		Card->SetOwnerRow(DeckP1, false);
		Card->FinishSpawning(FTransform::Identity);
	}

	// give cards to P2's Deck
	for (int i = 0; i < InitialDeckSize; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, SelectableCards.Num() - 1);
		const FCardData& SelectedCardData = SelectableCards[RandomIndex];
	
		// Spawn the card
		AGW_CardBase* Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->InitializeCardData(SelectedCardData);
		Card->Graveyard = GraveyardP2;
		Card->SetOwnerRow(DeckP2, false);
		Card->FinishSpawning(FTransform::Identity);
	}

	GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer, this, &AGW_GameMode::GiveDeckCardsToHands, 1.f);
}

void AGW_GameMode::GiveDeckCardsToHands()
{
	if (InitialHandSize <= InitialDeckSize)
	{
		DeckP1->GiveRandomCardsToHand(InitialHandSize);
		DeckP2->GiveRandomCardsToHand(InitialHandSize);
		StartPlayerTurn();
	}
}

void AGW_GameMode::SetGamePhase(EGamePhase NewPhase)
{
	CurrentGamePhase = NewPhase;
	
	switch (NewPhase)
	{
	case EGamePhase::Start:
		{
			// UKismetSystemLibrary::PrintString(this, "GamePhase = Start", true, true, FColor::Green, 30.f, FName("phase"));
			// if it is first round
			if (RoundIndex == 0)
			{
				if (bSpawnAIController)
				{
					const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector);
					AIController = GetWorld()->SpawnActorDeferred<AGW_AIController>(
						AGW_AIController::StaticClass(),
						SpawnTransform);
		
					AIController->WaitDuration = AIWaitDuration;
					AIController->FinishSpawning(SpawnTransform);
				}

				PlayerController = Cast<AGW_PlayerController>(GetWorld()->GetFirstPlayerController());
				GenerateRandomCardsForDeck();
			}
			else
			{
				Player1Data->ResetDataForNextRound();
				Player2Data->ResetDataForNextRound();

				ClearAllRows();

				GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer,[this]()
				{
					StartPlayerTurn();
				},
			StartGameWaitDuration,
			false);
			}

			OnNewRoundStarted.Broadcast();
			OnAnnouncementMessage.Broadcast("Round Start");
			break;
		}

		
	case EGamePhase::Player1Turn:
		{
			//UKismetSystemLibrary::PrintString(this, "GamePhase = Player1Turn", true, true, FColor::Green, 30.f, FName("phase"));

			PlayerController->StartTurn();
			OnAnnouncementMessage.Broadcast("Your turn!");
			break;
		}
		
	case EGamePhase::Wait:
		{
			//UKismetSystemLibrary::PrintString(this, "GamePhase = Wait", true, true, FColor::Green, 30.f, FName("phase"));

			Player1Data->SetIsPlayerTurn(false);
			Player2Data->SetIsPlayerTurn(false);
			GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer, this, &AGW_GameMode::StartPlayerTurn, WaitPhaseDuration);
			break;
		}

		
	case EGamePhase::Player2Turn:
		{
			// if Player 2 is an AI Controller and exists, start its turn
			if (bSpawnAIController)
			{
				AIController->StartTurn();
				//UKismetSystemLibrary::PrintString(this, "GamePhase = Player2Turn", true, true, FColor::Green, 30.f, FName("phase"));
			}
			else // if there is no AI controller, it will be Player 1's turn again
			{
				//UKismetSystemLibrary::PrintString(this, "No AI Controller");
				//SetGamePhase(EGamePhase::Player1Turn);
			}
			OnAnnouncementMessage.Broadcast("Opponent's turn");
			break;
		}


	case EGamePhase::RoundEnd:
		{
			//UKismetSystemLibrary::PrintString(this, "GamePhase = RoundEnd", true, true, FColor::Green, 30.f, FName("phase"));

			bool bMatchEnded = false;
			EMatchResult RoundResult = DetermineResult(bMatchEnded);
		
		// 	UKismetSystemLibrary::PrintString(this, 
		// "Round Result: " + FindObject<UEnum>(ANY_PACKAGE, TEXT("EMatchResult"), true)
		// 				   ->GetNameStringByValue(static_cast<int64>(RoundResult)), true, true, FColor::Green, 5.f, FName("result"));

			
			if (bMatchEnded)
			{
				// end the game after a wait
				GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer,[this]()
				{
					SetGamePhase(EGamePhase::MatchEnd);
				},
			EndGameWaitDuration,
			false);
			}
			else
			{
				// restart the game after a wait
				GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer,[this]()
				{
					SetGamePhase(EGamePhase::Start);
				},
			RestartGameWaitDuration,
			false);
			}

			Player1Data->SetIsPlayerTurn(false);
			Player2Data->SetIsPlayerTurn(false);
			RoundScoreData[Player1Data->Data.PlayerName][RoundIndex] = Player1Data->Data.Score;
			RoundScoreData[Player2Data->Data.PlayerName][RoundIndex] = Player2Data->Data.Score;

			RoundIndex++;

			// show Announcement Message
			switch (RoundResult)
			{
			case EMatchResult::Player1Wins:
				OnAnnouncementMessage.Broadcast("You won the round!");
				break;

			case EMatchResult::Player2Wins:
				OnAnnouncementMessage.Broadcast("Your opponent won the round");
				break;

			case EMatchResult::Draw:
				OnAnnouncementMessage.Broadcast("Draw");
				break;
			}
			
			break;
		}

	
	case EGamePhase::MatchEnd:
		{
			//UKismetSystemLibrary::PrintString(this, "GamePhase = MatchEnd", true, true, FColor::Green, 30.f, FName("phase"));
		
		// 	UKismetSystemLibrary::PrintString(this, 
		// "Final Match Result: " + FindObject<UEnum>(ANY_PACKAGE, TEXT("EMatchResult"), true)
		// 				   ->GetNameStringByValue(static_cast<int64>(FinalMatchResult)), true, true, FColor::Green, 30.f, FName("result"));

			// show Win-Lose Screen
			OnMatchEnd.Broadcast(FinalMatchResult, RoundScoreData);

			RoundIndex = 0;
			break;
		}
	}

	// print score
	//UKismetSystemLibrary::PrintString(this, "P1 SCORE: " + FString::FromInt(Player1Data->GetScore()), true, true, FColor::Cyan, 30.f, FName("p1score"));
	//UKismetSystemLibrary::PrintString(this, "P2 SCORE: " + FString::FromInt(Player2Data->GetScore()), true, true, FColor::Red, 30.f, FName("p2score"));
	//UKismetSystemLibrary::PrintString(this, "P1 HANDSIZE: " + FString::FromInt(Player1Data->GetHandSize()), true, true, FColor::Cyan, 30.f, FName("p1handsize"));
	//UKismetSystemLibrary::PrintString(this, "P2 HANDSIZE: " + FString::FromInt(Player2Data->GetHandSize()), true, true, FColor::Red, 30.f, FName("p2handsize"));
	//UKismetSystemLibrary::PrintString(this, "P1 LIVES: " + FString::FromInt(Player1Data->GetLifeLeft()), true, true, FColor::Cyan, 30.f, FName("p1lives"));
	//UKismetSystemLibrary::PrintString(this, "P2 LIVES: " + FString::FromInt(Player2Data->GetLifeLeft()), true, true, FColor::Red, 30.f, FName("p2lives"));
}

void AGW_GameMode::StartPlayerTurn()
{
	// decide which player should play next turn
	const bool bIsP1Turn = LastPlayedPlayerID == EPlayerID::Player2;
	if (bIsP1Turn)
	{
		SetGamePhase(EGamePhase::Player1Turn);
	}
	else
	{
		SetGamePhase(EGamePhase::Player2Turn);
	}
	Player1Data->SetIsPlayerTurn(bIsP1Turn);
	Player2Data->SetIsPlayerTurn(!bIsP1Turn);
}

EMatchResult AGW_GameMode::DetermineResult(bool& bMatchEnded)
{
	EMatchResult RoundResult = EMatchResult::Draw;

	if (Player1Data->GetScore() > Player2Data->GetScore())
	{
		Player2Data->DecrementLifeLeft();
		RoundResult = EMatchResult::Player1Wins;
	}
	else if (Player2Data->GetScore() > Player1Data->GetScore())
	{
		Player1Data->DecrementLifeLeft();
		RoundResult = EMatchResult::Player2Wins;
	}
	else
	{
		// In case of a draw, both players lose 1 life
		Player1Data->DecrementLifeLeft();
		Player2Data->DecrementLifeLeft();
	}

	// Check if the match should end
	if (Player1Data->GetLifeLeft() == 0 || Player2Data->GetLifeLeft() == 0)
	{
		FinalMatchResult = RoundResult;
		bMatchEnded = true;
	}

	return RoundResult;
}

void AGW_GameMode::ClearAllRows()
{
	for (AGW_UnitRow* Row : AllRowsArray)
	{
		for (AGW_CardBase* Card : Row->GetSnappedCardsArray())
		{
			Card->DestroySelf();
		}
		
		if (AGW_CardBase* SpecialCard = Row->GetSnappedSpecialCard())
		{
			SpecialCard->DestroySelf();
		}
	}
	
	for (AGW_CardBase* Card : WeatherRow->GetSnappedCardsArray())
	{
		Card->DestroySelf();
	}
}

void AGW_GameMode::OnHUDReady(AGW_HUD* HUD)
{
	// initialize player data widgets
	HUD->UpdatePlayerData(Player1Data, 1);
	HUD->UpdatePlayerData(Player2Data, 2);
}

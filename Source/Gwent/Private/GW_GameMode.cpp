// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "GW_AIController.h"
#include "GW_PlayerController.h"
#include "Card/GW_CardBase.h"
#include "Data/GW_CardDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Row/GW_Deck.h"
#include "Row/GW_Graveyard.h"
#include "Row/GW_PlayerHand.h"
#include "Row/GW_WeatherRow.h"


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
	LastPlayedID = PlayerID;
	SetGamePhase(EGamePhase::Wait);
}

void AGW_GameMode::PlayerPassedTurn(EPlayerID PlayerID)
{
	PlayerID == EPlayerID::Player1 ? Player1Data.PassedTurn = true : Player2Data.PassedTurn = true;

	// check end condition, if both passed turn then end the game/round
	if (Player1Data.PassedTurn && Player2Data.PassedTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer,[this]()
		{
			SetGamePhase(EGamePhase::End);
		},
	1.f,
	false);
	}
	else
	{
		EndPlayerTurn(PlayerID);
	}
}

void AGW_GameMode::SetPlayerHandSize(EPlayerID PlayerID, int32 HandSize)
{
	PlayerID == EPlayerID::Player1 ? Player1Data.HandSize = HandSize : Player2Data.HandSize = HandSize;
}

void AGW_GameMode::UpdatePlayerScore(const EPlayerID PlayerID)
{
	if (PlayerID == EPlayerID::Player1)
	{
		Player1Data.Score = CalculateScore(RowArrayP1);
	}
	else
	{
		Player2Data.Score = CalculateScore(RowArrayP2);
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

	// start the game
	SetGamePhase(EGamePhase::Start);
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
		SetGamePhase(EGamePhase::Player1Turn);
	}
}


void AGW_GameMode::SetGamePhase(EGamePhase NewPhase)
{
	CurrentGamePhase = NewPhase;
	
	switch (NewPhase)
	{
	case EGamePhase::Start:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Start", true, true, FColor::Green, 30.f, FName("phase"));

		GenerateRandomCardsForDeck();

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
		
		break;

		
	case EGamePhase::Player1Turn:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Player1Turn", true, true, FColor::Green, 30.f, FName("phase"));

		PlayerController->StartTurn();
		break;

		
	case EGamePhase::Wait:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Wait", true, true, FColor::Green, 30.f, FName("phase"));

		GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer, this, &AGW_GameMode::StartPlayerTurn, 1.f);
		break;

		
	case EGamePhase::Player2Turn:

		// if Player 2 is an AI Controller and exists, start its turn
		if (bSpawnAIController)
		{
			AIController->StartTurn();
			UKismetSystemLibrary::PrintString(this, "GamePhase = Player2Turn", true, true, FColor::Green, 30.f, FName("phase"));
		}
		else // if there is no AI controller, it will be Player 1's turn again
		{
			UKismetSystemLibrary::PrintString(this, "No AI Controller");
			SetGamePhase(EGamePhase::Player1Turn);
		}
		break;

		
	case EGamePhase::End:
		UKismetSystemLibrary::PrintString(this, "GamePhase = End", true, true, FColor::Green, 30.f, FName("phase"));

		EMatchResult MatchResult = DetermineMatchResult();
		
		UKismetSystemLibrary::PrintString(this, 
	"Match Result: " + FindObject<UEnum>(ANY_PACKAGE, TEXT("EMatchResult"), true)
					   ->GetNameStringByValue(static_cast<int64>(MatchResult)), true, true, FColor::Green, 30.f, FName("phase"));

		//TODO: Win-Lose screens
		break;
	}

	// print score
	UKismetSystemLibrary::PrintString(this, "P1 SCORE: " + FString::FromInt(Player1Data.Score), true, true, FColor::Cyan, 30.f, FName("p1score"));
	UKismetSystemLibrary::PrintString(this, "P2 SCORE: " + FString::FromInt(Player2Data.Score), true, true, FColor::Red, 30.f, FName("p2score"));
	UKismetSystemLibrary::PrintString(this, "P1 HANDSIZE: " + FString::FromInt(Player1Data.HandSize), true, true, FColor::Cyan, 30.f, FName("p1handsize"));
	UKismetSystemLibrary::PrintString(this, "P2 HANDSIZE: " + FString::FromInt(Player2Data.HandSize), true, true, FColor::Red, 30.f, FName("p2handsize"));
}

void AGW_GameMode::StartPlayerTurn()
{
	// decide which player should play next turn
	if (LastPlayedID == EPlayerID::Player1)
	{
		SetGamePhase(EGamePhase::Player2Turn);
	}
	else if (LastPlayedID == EPlayerID::Player2)
	{
		SetGamePhase(EGamePhase::Player1Turn);
	}
}

EMatchResult AGW_GameMode::DetermineMatchResult()
{
	if (Player1Data.Score > Player2Data.Score)
	{
		return EMatchResult::Player1Wins;
	}

	if (Player2Data.Score > Player1Data.Score)
	{
		return EMatchResult::Player2Wins;
	}

	return EMatchResult::Draw;
}

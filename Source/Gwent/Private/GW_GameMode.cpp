﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "GW_AIController.h"
#include "Card/GW_CardBase.h"
#include "Data/GW_CardDataAsset.h"
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
	for (int i = 0; i < DeckSize; ++i)
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
	for (int i = 0; i < DeckSize; ++i)
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

	SetGamePhase(EGamePhase::Player1Turn);
}

void AGW_GameMode::SetGamePhase(EGamePhase NewPhase)
{
	CurrentGamePhase = NewPhase;
	
	switch (NewPhase)
	{
	case EGamePhase::Start:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Start");

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
		break;

		
	case EGamePhase::Player1Turn:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Player1Turn");
		break;

		
	case EGamePhase::Wait:
		UKismetSystemLibrary::PrintString(this, "GamePhase = Wait");

		GetWorld()->GetTimerManager().SetTimer(WaitPhaseTimer, this, &AGW_GameMode::StartPlayerTurn, 1.f);
		break;

		
	case EGamePhase::Player2Turn:

		// if Player 2 is an AI Controller and exists, start its turn
		if (bSpawnAIController)
		{
			AIController->StartTurn();
			UKismetSystemLibrary::PrintString(this, "GamePhase = Player2Turn");
		}
		else // if there is no AI controller, it will be Player 1's turn again
		{
			UKismetSystemLibrary::PrintString(this, "No AI Controller");
			SetGamePhase(EGamePhase::Player1Turn);
		}
		break;

		
	case EGamePhase::End:
		UKismetSystemLibrary::PrintString(this, "GamePhase = End");

		//TODO: Win-Lose screens
		break;
	}
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

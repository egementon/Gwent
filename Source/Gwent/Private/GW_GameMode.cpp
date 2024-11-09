// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "Card/GW_CardBase.h"
#include "Data/GW_CardDataAsset.h"
#include "Row/GW_Deck.h"
#include "Row/GW_Graveyard.h"
#include "Row/GW_PlayerHand.h"


void AGW_GameMode::RegisterRow(AGW_RowBase* NewRow)
{
	if (AGW_UnitRow* NewUnitRow = Cast<AGW_UnitRow>(NewRow))
	{
		this->RowArray.Add(NewUnitRow);
	}
	else if (AGW_Deck* NewDeck = Cast<AGW_Deck>(NewRow))
	{
		this->Deck = NewDeck;
	}
	else if (AGW_PlayerHand* NewPlayerHand = Cast<AGW_PlayerHand>(NewRow))
	{
		this->PlayerHand = NewPlayerHand;
	}
	else if (AGW_Graveyard* NewGraveyard = Cast<AGW_Graveyard>(NewRow))
	{
		this->Graveyard = NewGraveyard;
	}
}

void AGW_GameMode::BeginPlay()
{
	Super::BeginPlay();
	GenerateRandomCardsForDeck();
}

void AGW_GameMode::GenerateRandomCardsForDeck()
{
	if (!CardDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No card data available."));
		return;
	}
	
	TArray<FCardData> SelectableCards = CardDataAsset->CardDataArray;

	// remove excluded cards from SelectableCards
	for (int32 i = SelectableCards.Num() - 1; i >= 0; --i)
	{
		if (SelectableCards[i].bDebug_ExcludeFromDeck == true)
		{
			SelectableCards.RemoveAt(i);
		}
	}

	if (SelectableCards.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No suitable card."));
		return;
	}
	
	for (int i = 0; i < DeckSize; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, SelectableCards.Num() - 1);
		const FCardData& SelectedCardData = SelectableCards[RandomIndex];

		// Spawn the card
		AGW_CardBase* Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->InitializeCardData(SelectedCardData);
		Card->Graveyard = Graveyard;
		Card->SetOwnerRow(Deck, false);
		Card->FinishSpawning(FTransform::Identity);
	}
}

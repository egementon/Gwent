﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_PlayerDeck.h"

#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"


AGW_PlayerDeck::AGW_PlayerDeck()
{
	bIsPlayerDeck = true;

	TotalPowerText->SetVisibility(false);
	SpecialSlotBoxComponent->SetVisibility(false);
}


void AGW_PlayerDeck::GenerateRandomCards()
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
	
	for (int i = 0; i < 10; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, SelectableCards.Num() - 1);
		const FCardData& SelectedCardData = SelectableCards[RandomIndex];

		// Spawn the card
		auto Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->InitializeCardData(SelectedCardData);
		Card->SetOwnerRow(this, false);
		Card->FinishSpawning(FTransform::Identity);
		SnappedCardsArray.AddUnique(Card);
	}

	UpdateCardsLocations();
}

void AGW_PlayerDeck::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateRandomCards();
}



// Fill out your copyright notice in the Description page of Project Settings.


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
	if (!CardDataAsset || CardDataAsset->CardDataArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No card data available."));
		return;
	}
	
	for (int i = 0; i < 10; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, CardDataAsset->CardDataArray.Num() - 1);
		const FCardData& SelectedCardData = CardDataAsset->CardDataArray[RandomIndex];

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



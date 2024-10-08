// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_PlayerDeck.h"

#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


AGW_PlayerDeck::AGW_PlayerDeck()
{
	PrimaryActorTick.bCanEverTick = false;

	RowBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RowBoxComponent"));
	RowBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGW_PlayerDeck::AddToCardsArray(AGW_CardBase* AddedCard)
{
	SnappedCardsArray.AddUnique(AddedCard);
	UpdateCardsLocations();
}

void AGW_PlayerDeck::RemoveFromCardsArray(AGW_CardBase* RemovedCard)
{
	SnappedCardsArray.Remove(RemovedCard);
	UpdateCardsLocations();
}

void AGW_PlayerDeck::UpdateCardsLocations()
{
	int32 NumCards = SnappedCardsArray.Num();
	if (NumCards < 1) return;

	float CardSpacing = 80.f;
	FVector RowCenter = GetActorLocation();
	
	// Calculate starting position (center minus half of the total width of all cards)
	float TotalWidth = (NumCards - 1) * CardSpacing; // Total width including spacing
	FVector StartPosition = RowCenter - FVector(TotalWidth / 2.0f, 0, 0); // Adjust X axis

	// Loop through each card and position them accordingly.
	for (int32 i = 0; i < NumCards; i++)
	{
		if (SnappedCardsArray[i])
		{
			FVector NewPosition = StartPosition + FVector(i * CardSpacing, 0, 0);
			SnappedCardsArray[i]->SetActorLocation(NewPosition);
		}
	}
}

void AGW_PlayerDeck::GenerateRandomCards()
{
	for (int i = 0; i < 10; ++i)
	{
		auto Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->CardPower = UKismetMathLibrary::RandomInteger(11);
		//Card->bIsSnapped = true;
		SnappedCardsArray.AddUnique(Card);
	}

	UpdateCardsLocations();
}

void AGW_PlayerDeck::BeginPlay()
{
	GenerateRandomCards();
}



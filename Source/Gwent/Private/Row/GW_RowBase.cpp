// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_RowBase.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"


// Sets default values
AGW_RowBase::AGW_RowBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RowBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RowBoxComponent"));
	RowBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RowBoxComponent->ComponentTags.Add(FName("RowSlot"));
}

TArray<AGW_CardBase*> AGW_RowBase::GetSnappedCardsArray()
{
	return SnappedCardsArray;
}

EPlayerID AGW_RowBase::GetPlayerID()
{
	return PlayerID;
}

void AGW_RowBase::AddToCardsArray(AGW_CardBase* AddedCard)
{
	SnappedCardsArray.AddUnique(AddedCard);
	UpdateCardsLocations();
}

void AGW_RowBase::RemoveFromCardsArray(AGW_CardBase* RemovedCard)
{
	SnappedCardsArray.Remove(RemovedCard);
	UpdateCardsLocations();
}

void AGW_RowBase::UpdateCardsLocations()
{
	int32 NumCards = SnappedCardsArray.Num();
	if (NumCards < 1) return;

	FVector RowCenter = GetActorLocation();

	// Calculate starting position (center minus half of the total width of all cards)
	float TotalWidth = (NumCards - 1) * CardSpacingX; // Total width including spacing
	FVector StartPosition = RowCenter - FVector(TotalWidth / 2.0f, 0, 0); // Adjust X axis

	// Loop through each card and position them accordingly.
	for (int32 i = 0; i < NumCards; i++)
	{
		if (SnappedCardsArray[i])
		{
			FVector NewPosition = StartPosition + FVector(i * CardSpacingX, 0, 0);
			SnappedCardsArray[i]->SetActorLocation(NewPosition);
		}
	}

	// Apply specific positioning if bVerticalOffset is true
	if (bVerticalOffset)
	{
		// Place the first card at the row's center
		if (SnappedCardsArray[0])
		{
			SnappedCardsArray[0]->SetActorLocation(RowCenter);
		}

		// Position the rest of the cards to the right of the first one
		for (int32 i = 1; i < NumCards; i++)
		{
			if (SnappedCardsArray[i])
			{
				FVector RightOffset = RowCenter + FVector(i * CardSpacingX, 0, i * CardSpacingZ);
				SnappedCardsArray[i]->SetActorLocation(RightOffset);
			}
		}
	}
}

void AGW_RowBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGW_RowBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameMode = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->RegisterRow(this);
	}
}

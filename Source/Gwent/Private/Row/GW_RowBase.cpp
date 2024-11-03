// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_RowBase.h"

#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"


// Sets default values
AGW_RowBase::AGW_RowBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RowBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RowBoxComponent"));
	RowBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

TArray<AGW_CardBase*> AGW_RowBase::GetSnappedCardsArray()
{
	return SnappedCardsArray;
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

	float CardSpacing = 107.f;
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

void AGW_RowBase::BeginPlay()
{
	Super::BeginPlay();
}


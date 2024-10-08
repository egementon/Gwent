// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_Row.h"

#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"


// Sets default values
AGW_Row::AGW_Row()
{
	PrimaryActorTick.bCanEverTick = false;

	RowBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RowBoxComponent"));
	RowBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!bIsPlayerDeck)
	{
		TotalPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
		TotalPowerText->SetupAttachment(RowBoxComponent);
	}
}

void AGW_Row::AddToCardsArray(AGW_CardBase* AddedCard)
{
	SnappedCardsArray.AddUnique(AddedCard);
	UpdateCardsLocations();
	if (!bIsPlayerDeck) CalculateTotalPower();
}

void AGW_Row::RemoveFromCardsArray(AGW_CardBase* RemovedCard)
{
	SnappedCardsArray.Remove(RemovedCard);
	UpdateCardsLocations();
	if (!bIsPlayerDeck) CalculateTotalPower();
}

void AGW_Row::UpdateCardsLocations()
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

void AGW_Row::CalculateTotalPower()
{
	TotalPower = 0;
	if (!SnappedCardsArray.IsEmpty())
	{
		for (AGW_CardBase* Card : SnappedCardsArray)
		{
			TotalPower += Card->CardPower;
		}
	}

	TotalPowerText->SetText(FText::AsNumber(TotalPower));
}

void AGW_Row::BeginPlay()
{
}


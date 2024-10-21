// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_Row.h"

#include "GW_GameMode.h"
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

		SpecialSlotBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SpecialSlot"));
		SpecialSlotBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpecialSlotBoxComponent->SetupAttachment(RowBoxComponent);
	}
}

TArray<AGW_CardBase*> AGW_Row::GetSnappedCardsArray()
{
	return SnappedCardsArray;
}

AGW_CardBase* AGW_Row::GetSnappedSpecialCard()
{
	return SnappedSpecialCard;
}

bool AGW_Row::IsSpecialSlotEmpty()
{
	return bIsSpecialSlotEmpty;
}

bool AGW_Row::IsPlayerDeck()
{
	return bIsPlayerDeck;
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

bool AGW_Row::IsValidRowForCard(AGW_CardBase* Card)
{
	if (bIsPlayerDeck)
	{
		return false;
	}
	
	if (Card->bIsSpecial && !bIsSpecialSlotEmpty)
	{
		return false;
	}

	const ECardRowType CardRowType = Card->CardRowType;
	
	if (CardRowType == ECardRowType::AllRows)
	{
		return true;
	}
	
	if (this->RowType == CardRowType)
	{
		return true;
	}
	
	if (CardRowType == ECardRowType::Agile && (this->RowType == ECardRowType::Melee || this->RowType == ECardRowType::Ranged))
	{
		return true;
	}
	
	return false;
}

void AGW_Row::SetSpecialCard(AGW_CardBase* SpecialCard)
{
	if (SpecialCard)
	{
		SpecialCard->SetActorLocation(SpecialSlotBoxComponent->GetComponentLocation());
	}
	SnappedSpecialCard = SpecialCard;
	bIsSpecialSlotEmpty = false;
}

void AGW_Row::SetSpecialSlotEmpty(bool bIsEmpty)
{
	bIsSpecialSlotEmpty = bIsEmpty;
}

void AGW_Row::BeginPlay()
{
	// add self reference to GameMode
	if (!bIsPlayerDeck)
	{
		Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->RowArray.AddUnique(this);
	}
	else
	{
		Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerDeck = this;
	}
}


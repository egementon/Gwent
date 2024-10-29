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
	
	if (!bIsPlayerHand)
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

bool AGW_Row::IsPlayerHand()
{
	return bIsPlayerHand;
}

void AGW_Row::AddToCardsArray(AGW_CardBase* AddedCard)
{
	SnappedCardsArray.AddUnique(AddedCard);
	UpdateCardsLocations();

	if (bIsPlayerHand) return; // If it is player hand, do not calculate any power
	UpdateAllCardsPowers();
	CalculateRowPower();
}

void AGW_Row::RemoveFromCardsArray(AGW_CardBase* RemovedCard)
{
	SnappedCardsArray.Remove(RemovedCard);
	UpdateCardsLocations();
	
	if (bIsPlayerHand) return; // If it is player hand, do not calculate any power
	UpdateAllCardsPowers();
	CalculateRowPower();
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

void AGW_Row::SetCardPowerParameters(AGW_CardBase* AddedCard)
{
	// Update Card's power calculation parameters according to Row
	if (bRowHasBadWeather)
	{
		AddedCard->bHasWeatherDamage = true;
	}
	else
	{
		AddedCard->bHasWeatherDamage = false;
	}

	if (RowMoraleBoostAddition > 0)
	{
		int32 Addition = RowMoraleBoostAddition;
		if (AddedCard->GetCardAbility() == ECardAbility::MoraleBoost)
		{
			--Addition;
		}
		
		AddedCard->MoraleBoost = Addition;
	}

	if (AddedCard->GetCardAbility() == ECardAbility::TightBond)
	{
		if (const TArray<AGW_CardBase*>* BondedArray = TightBondedCards.Find(AddedCard->GetCardName()))
		{
			const int32 BondedCount = BondedArray->Num();
			AddedCard->TightBondMultiplier = BondedCount;
		}
	}
	
	if (bRowHasHorn)
	{
		AddedCard->bHasHornBoost = true;
	}
	else
	{
		AddedCard->bHasHornBoost = false;
	}

	// calculate a card's power based on set parameters
	AddedCard->CalculatePower();
}

void AGW_Row::UpdateAllCardsPowers()
{
	if (!SnappedCardsArray.IsEmpty())
	{
		for (AGW_CardBase* Card : SnappedCardsArray)
		{
			SetCardPowerParameters(Card);
		}
	}
}

void AGW_Row::CalculateRowPower()
{
	TotalPower = 0;
	if (!SnappedCardsArray.IsEmpty())
	{
		for (AGW_CardBase* Card : SnappedCardsArray)
		{
			TotalPower += Card->GetCardPower();
		}
	}

	TotalPowerText->SetText(FText::AsNumber(TotalPower));
}

bool AGW_Row::IsValidRowForCard(AGW_CardBase* Card)
{
	if (bIsPlayerHand)
	{
		return false;
	}
	
	if (Card->bIsSpecial && !bIsSpecialSlotEmpty)
	{
		return false;
	}

	const ECardRowType CardRowType = Card->GetCardRowType();
	
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
	UpdateAllCardsPowers();
	CalculateRowPower();
}

void AGW_Row::SetSpecialSlotEmpty(bool bIsEmpty)
{
	bIsSpecialSlotEmpty = bIsEmpty;
}

void AGW_Row::OnTightBondedCardRemoved(AGW_CardBase* DestroyedCard)
{
	// TODO: Test this with Decoy card when added
	if (TArray<AGW_CardBase*>* BondedArray = TightBondedCards.Find(DestroyedCard->GetCardName()))
	{
		// Remove the card from the array
		BondedArray->Remove(DestroyedCard);

		UE_LOG(LogTemp, Log, TEXT("Removed %s from bonded array. Remaining cards: %d"), *DestroyedCard->GetCardName().ToString(), BondedArray->Num());

		// If the array is now empty, remove the entry from the map
		if (BondedArray->Num() == 0)
		{
			TightBondedCards.Remove(DestroyedCard->GetCardName());
			UE_LOG(LogTemp, Log, TEXT("Bonded array for %s is now empty and has been removed from the map."), *DestroyedCard->GetCardName().ToString());
		}
	}
}

void AGW_Row::BeginPlay()
{
	// add self reference to GameMode
	if (!bIsPlayerHand)
	{
		Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->RowArray.AddUnique(this);
	}
	else
	{
		Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerHand = this;
	}
}


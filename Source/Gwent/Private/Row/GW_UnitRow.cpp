// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_UnitRow.h"

#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"


// Sets default values
AGW_UnitRow::AGW_UnitRow()
{
	PrimaryActorTick.bCanEverTick = false;
	
	TotalPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TotalPowerText->SetupAttachment(RowBoxComponent);

	SpecialSlotBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SpecialSlot"));
	SpecialSlotBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpecialSlotBoxComponent->SetupAttachment(RowBoxComponent);
	SpecialSlotBoxComponent->ComponentTags.Add(FName("SpecialSlot"));

	// enable line trace for these box components
	RowBoxComponent->SetCollisionProfileName("Row");
	SpecialSlotBoxComponent->SetCollisionProfileName("Row");
}

void AGW_UnitRow::BeginPlay()
{
	// add self reference to GameMode
	Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->RowArray.AddUnique(this);
}

AGW_CardBase* AGW_UnitRow::GetSnappedSpecialCard()
{
	return SnappedSpecialCard;
}

bool AGW_UnitRow::IsSpecialSlotEmpty()
{
	return bIsSpecialSlotEmpty;
}

void AGW_UnitRow::SetSpecialCard(AGW_CardBase* SpecialCard)
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

void AGW_UnitRow::SetSpecialSlotEmpty(bool bIsEmpty)
{
	bIsSpecialSlotEmpty = bIsEmpty;
}

void AGW_UnitRow::SetCardPowerParameters(AGW_CardBase* AddedCard)
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

void AGW_UnitRow::UpdateAllCardsPowers()
{
	if (!SnappedCardsArray.IsEmpty())
	{
		for (AGW_CardBase* Card : SnappedCardsArray)
		{
			SetCardPowerParameters(Card);
		}
	}
}

void AGW_UnitRow::CalculateRowPower()
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

bool AGW_UnitRow::IsValidRowForCard(AGW_CardBase* Card)
{
	if (Card->bIsSpecial && !bIsSpecialSlotEmpty)
	{
		return false;
	}

	const EUnitRowType CardRowType = Card->GetCardRowType();
	
	if (CardRowType == EUnitRowType::AllRows)
	{
		return true;
	}
	
	if (this->RowType == CardRowType)
	{
		return true;
	}
	
	if (CardRowType == EUnitRowType::Agile && (this->RowType == EUnitRowType::Melee || this->RowType == EUnitRowType::Ranged))
	{
		return true;
	}
	
	return false;
}

void AGW_UnitRow::AddToCardsArray(AGW_CardBase* AddedCard)
{
	Super::AddToCardsArray(AddedCard);
	UpdateAllCardsPowers();
	CalculateRowPower();
}

void AGW_UnitRow::RemoveFromCardsArray(AGW_CardBase* RemovedCard)
{
	Super::RemoveFromCardsArray(RemovedCard);
	UpdateAllCardsPowers();
	CalculateRowPower();
}

void AGW_UnitRow::OnTightBondedCardRemoved(AGW_CardBase* DestroyedCard)
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


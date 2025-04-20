// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_UnitRow.h"

#include "GW_FuncLib.h"
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

	// area meshes to display outline effect on rows
	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));
	AreaMesh->SetupAttachment(RootComponent);
	AreaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaMesh->SetRenderInMainPass(false);
	AreaMesh->SetRenderInDepthPass(false);
	AreaMesh->SetCastShadow(false);
	SpecialSlotAreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpecialSlotAreaMesh"));
	SpecialSlotAreaMesh->SetupAttachment(RootComponent);
	SpecialSlotAreaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpecialSlotAreaMesh->SetRenderInMainPass(false);
	SpecialSlotAreaMesh->SetRenderInDepthPass(false);
	SpecialSlotAreaMesh->SetCastShadow(false);
	BadWeatherAreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BadWeatherAreaMesh"));
	BadWeatherAreaMesh->SetupAttachment(RootComponent);
	BadWeatherAreaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BadWeatherAreaMesh->SetVisibility(false);
}

void AGW_UnitRow::BeginPlay()
{
	Super::BeginPlay();
}

AGW_CardBase* AGW_UnitRow::GetSnappedSpecialCard()
{
	return SnappedSpecialCard;
}

bool AGW_UnitRow::IsSpecialSlotEmpty()
{
	return bIsSpecialSlotEmpty;
}

int32 AGW_UnitRow::GetTotalPower()
{
	return TotalPower;
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

void AGW_UnitRow::SetRowHasBadWeather(bool bHasBadWeather)
{
	bRowHasBadWeather = bHasBadWeather;
	BadWeatherAreaMesh->SetVisibility(bHasBadWeather);
}

// Update Card's power calculation parameters according to Row
void AGW_UnitRow::SetCardPowerParameters(AGW_CardBase* Card)
{
	// Bad Weather
	if (bRowHasBadWeather)
	{
		Card->bHasWeatherDamage = true;
	}
	else
	{
		Card->bHasWeatherDamage = false;
	}

	// Morale Boost
	int32 Addition = RowMoraleBoostAddition;

	// MoraleBoost cards can not boost themselves
	if (Card->GetCardAbility() == ECardAbility::MoraleBoost)
	{
		--Addition;
	}
	
	Card->MoraleBoost = Addition;

	// Tight Bond
	if (Card->GetCardAbility() == ECardAbility::TightBond)
	{
		if (const TArray<AGW_CardBase*>* BondedArray = TightBondedCards.Find(Card->GetCardName()))
		{
			const int32 BondedCount = BondedArray->Num();
			Card->TightBondMultiplier = BondedCount;
		}
	}

	// Horn
	if (bRowHasHorn)
	{
		Card->bHasHornBoost = true;
	}
	else
	{
		Card->bHasHornBoost = false;
	}

	// calculate a card's power based on set parameters
	Card->CalculatePower();
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

	if (TotalPowerText)
	{
		TotalPowerText->SetText(FText::AsNumber(TotalPower));
	}

	GameMode->UpdatePlayerScore(this->PlayerID);
}

bool AGW_UnitRow::IsValidRowForCard(AGW_CardBase* Card)
{
	// for weather cards
	if (Card->IsWeatherCard())
	{
		if (this->bIsWeatherRow)
		{
			return true;	
		}
		
		return false;
	}

	// exception is that spy cards can only be placed on opponent rows
	bool bIsSpyCard = Card->GetCardAbility() == ECardAbility::Spy;
	bool bDoesIDMatch = GetPlayerID() == Card->PlayerID;

	if (bIsSpyCard && bDoesIDMatch || !bIsSpyCard && !bDoesIDMatch)
	{
		return false;
	}
	
	if (Card->bIsSpecial && !bIsSpecialSlotEmpty)
	{
		return false;
	}

	const EUnitRowType CardRowType = Card->GetCardRowType();
	
	if (CardRowType == EUnitRowType::AllRows && !this->bIsWeatherRow) // can be placed on all unit rows
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

void AGW_UnitRow::HighlightRow(bool bHighlight)
{
	AreaMesh->SetRenderCustomDepth(bHighlight);
}

void AGW_UnitRow::HighlightRowSpecialSlot(bool bHighlight)
{
	if (SpecialSlotAreaMesh)
	{
		SpecialSlotAreaMesh->SetRenderCustomDepth(bHighlight);
	}
}


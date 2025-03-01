// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_WeatherRow.h"

#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"


AGW_WeatherRow::AGW_WeatherRow()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsWeatherRow = true;

	// does not need these components
	if (TotalPowerText)
	{
		TotalPowerText->DestroyComponent();
	}
	if (SpecialSlotBoxComponent)
	{
		SpecialSlotBoxComponent->DestroyComponent();
	}
	if (SpecialSlotAreaMesh)
	{
		SpecialSlotAreaMesh->DestroyComponent();
	}
}

void AGW_WeatherRow::BeginPlay()
{
	Super::BeginPlay();
}

void AGW_WeatherRow::AddToCardsArray(AGW_CardBase* AddedCard)
{
	bool bRowHasSameNameCard = false;

	for (AGW_CardBase* Card : SnappedCardsArray)
	{
		if (AddedCard->GetCardName() == Card->GetCardName())
		{
			bRowHasSameNameCard = true;
			break;
		}
	}

	if (bRowHasSameNameCard)
	{
		AddedCard->DestroySelf();
	}
	else
	{
		Super::AddToCardsArray(AddedCard);
		AddedCard->SetIsSelectable(false);
	}
}

void AGW_WeatherRow::DestroyBadWeatherCards()
{
	if (SnappedCardsArray.IsEmpty()) return;
	for (AGW_CardBase* Card : SnappedCardsArray)
	{
		if (Card->GetCardAbility() == ECardAbility::BadWeather)
		{
			Card->DestroySelfAfterDelay(0.15f);
		}
	}
}

void AGW_WeatherRow::ClearWeather()
{
	TArray<AGW_UnitRow*> RowArray = GameMode->AllRowsArray;
	for (AGW_UnitRow* Row : RowArray)
	{
		Row->SetRowHasBadWeather(false);
		Row->UpdateAllCardsPowers();
	}
}

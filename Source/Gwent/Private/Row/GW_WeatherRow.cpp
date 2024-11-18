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
		TotalPowerText->SetVisibility(false);
		TotalPowerText->SetHiddenInGame(true);
	}

	if (SpecialSlotBoxComponent)
	{
		SpecialSlotBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpecialSlotBoxComponent->SetVisibility(false);
	}
}

void AGW_WeatherRow::BeginPlay()
{
	Super::BeginPlay();
}

void AGW_WeatherRow::AddToCardsArray(AGW_CardBase* AddedCard)
{
	Super::AddToCardsArray(AddedCard);
	
	AddedCard->SetIsSelectable(false);

	// avoid stacking multiples of the same card by destroying the other same name card
	if (SnappedCardsArray.IsEmpty()) return;
	for (AGW_CardBase* Card : SnappedCardsArray)
	{
		if (Card == AddedCard)
		{
			continue;
		}
		
		if (AddedCard->GetCardName() == Card->GetCardName())
		{
			Card->DestroySelf();
			break;
		}
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
	TArray<AGW_UnitRow*> RowArray = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->AllRowsArray;
	for (AGW_UnitRow* Row : RowArray)
	{
		Row->bRowHasBadWeather = false;
		Row->UpdateAllCardsPowers();
	}
}

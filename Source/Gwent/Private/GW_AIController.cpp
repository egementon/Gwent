﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_AIController.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Row/GW_PlayerHand.h"


AGW_AIController::AGW_AIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGW_AIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHandP2 = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerHandP2;
	RowArrayP2 = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->RowArrayP2;
	WeatherRow = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->WeatherRow;

	// play cards periodically 
	//GetWorld()->GetTimerManager().SetTimer(WaitBeforePlayTimer, this, &AGW_AIController::PlayRandomCard, WaitDuration, true, 2.f);
}

void AGW_AIController::StartTurn()
{
	GetWorld()->GetTimerManager().SetTimer(WaitBeforePlayTimer, this, &AGW_AIController::PlayRandomCard, WaitDuration);
}

void AGW_AIController::PlayRandomCard()
{
	TArray<AGW_CardBase*> HandCards = PlayerHandP2->GetSnappedCardsArray();
	if (HandCards.IsEmpty()) return;

	AGW_UnitRow* ValidRow = nullptr;

	// select random card from Hand
	const int32 RandomIndex = FMath::RandRange(0, HandCards.Num() - 1);
	AGW_CardBase* SelectedCard = HandCards[RandomIndex];

	// place on the WeatherRow if it is a WeatherCard
	if (SelectedCard->IsWeatherCard())
	{
		ValidRow = Cast<AGW_UnitRow>(WeatherRow);
	}
	else
	{
		// find first valid row (for now)
		for (AGW_UnitRow* Row : RowArrayP2)
		{
			if (Row->IsValidRowForCard(SelectedCard))
			{
				ValidRow = Row;
				break;
			}
		}
	}
	
	SelectedCard->DetachFromOwnerRow();
	SelectedCard->SetOwnerRow(ValidRow, true);

	UGW_FuncLib::GetGameMode(GetWorld())->EndPlayerTurn(PlayerControllerID);
}


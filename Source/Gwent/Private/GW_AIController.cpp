// Fill out your copyright notice in the Description page of Project Settings.


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

	if (const AGW_GameMode* GameMode = UGW_FuncLib::GetGameMode(GetWorld()))
	{
		PlayerHandP2 = GameMode->PlayerHandP2;
		RowArrayP2 = GameMode->RowArrayP2;
		WeatherRow = GameMode->WeatherRow;
	}

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

	if (UGW_FuncLib::GetGameMode(GetWorld())->Player2Data.HandSize == 0)
	{
		// pass turn if no cards left in the hand
		UGW_FuncLib::GetGameMode(GetWorld())->PlayerPassedTurn(PlayerControllerID);
	}
	else
	{
		UGW_FuncLib::GetGameMode(GetWorld())->EndPlayerTurn(PlayerControllerID);
	}
}


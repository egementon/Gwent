// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_AIController.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Row/GW_PlayerHand.h"

/*	Current AI Behaviour:
 *	Play a random card to the first found valid row
 *	If no cards left in the hand, pass turn
 *	If opponent has passed, play a card then pass next turn
 */

AGW_AIController::AGW_AIController(): GameMode(nullptr), bReadyToPass(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGW_AIController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = UGW_FuncLib::GetGameMode(GetWorld());
	if (GameMode)
	{
		PlayerHandP2 = GameMode->PlayerHandP2;
		RowArrayP2 = GameMode->RowArrayP2;
		WeatherRow = GameMode->WeatherRow;
		
		GameMode->OnNewRoundStarted.AddLambda([this]()
		{
			bReadyToPass = false; // reset bReadyToPass on every round start
		});
	}

	// play cards periodically 
	//GetWorld()->GetTimerManager().SetTimer(WaitBeforePlayTimer, this, &AGW_AIController::PlayRandomCard, WaitDuration, true, 2.f);
}

void AGW_AIController::StartTurn()
{
	GetWorld()->GetTimerManager().SetTimer(WaitBeforePlayTimer, this, &AGW_AIController::MakeDecision, WaitDuration);
}

void AGW_AIController::MakeDecision()
{
	// if no cards left in the hand, pass your turn
	if (bReadyToPass || GameMode->Player2Data.HandSize == 0)
	{
		GameMode->PlayerPassedTurn(PlayerControllerID);
	}
	else
	{
		PlayRandomCard();
	}

	// if opponent has passed, ready yourself to pass next turn (play a card now and pass next turn)
	if (GameMode->Player1Data.PassedTurn)
	{
		bReadyToPass = true;
	}
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

	GameMode->EndPlayerTurn(PlayerControllerID);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_AIController.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Data/GW_PlayerData.h"
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
	if (bReadyToPass || GameMode->Player2Data->GetHandSize() == 0)
	{
		GameMode->PlayerPassedTurn(PlayerControllerID);
	}
	else
	{
		PlayRandomCard();
	}

	// if opponent has passed, ready yourself to pass next turn (play a card now and pass next turn)
	if (GameMode->Player1Data->IsTurnPassed())
	{
		bReadyToPass = true;
	}
}

AGW_CardBase* AGW_AIController::GetRandomCard()
{
	TArray<AGW_CardBase*> HandCards = PlayerHandP2->GetSnappedCardsArray();
	if (HandCards.IsEmpty()) return nullptr;

	// select random card from Hand
	const int32 RandomIndex = FMath::RandRange(0, HandCards.Num() - 1);
	return HandCards[RandomIndex];
}

void AGW_AIController::PlayRandomCard()
{
	AGW_CardBase* SelectedCard = GetRandomCard();

	if (SelectedCard->GetCardAbility() == ECardAbility::Decoy)
	{
		OnSelectedDecoyCard(SelectedCard);
	}
	else
	{
		PlayCard(SelectedCard);
	}
}

void AGW_AIController::PlayCard(AGW_CardBase* CardToPlay)
{
	CardToPlay->OnCardAbilityEnded.AddLambda([this]()
		{
			GameMode->EndPlayerTurn(PlayerControllerID);
		});
		
	CardToPlay->DetachAndSetOwnerRow(CardToPlay->FindValidRow(), true);
}

void AGW_AIController::OnSelectedDecoyCard(AGW_CardBase* SelectedCard)
{
	TArray<AGW_CardBase*> PlayedUnitCards;
	for (AGW_UnitRow* Row : RowArrayP2)
	{
		TArray<AGW_CardBase*> SnappedCards = Row->GetSnappedCardsArray();
		for (AGW_CardBase* Card : SnappedCards)
		{
			if (Card->IsRegularUnitCard())
			{
				PlayedUnitCards.Add(Card);
			}
		}
	}
	
	if (!PlayedUnitCards.IsEmpty())
	{
		const int32 RandomCardIndex = FMath::RandRange(0, PlayedUnitCards.Num() - 1);
		AGW_CardBase* CardToGetBack = PlayedUnitCards[RandomCardIndex];

		SelectedCard->DetachAndSetOwnerRow(CardToGetBack->GetOwnerRow(), true);
			
		CardToGetBack->DetachAndSetOwnerRow(GameMode->PlayerHandP2, false);
		CardToGetBack->SetIsSelectable(true);
		CardToGetBack->ResetCardPower();

		GameMode->EndPlayerTurn(PlayerControllerID);
	}
	else // if there is no card to swap with, select another card instead of Decoy
	{
		bool bFoundCardToPlay = false;
		for (AGW_CardBase* HandCard : PlayerHandP2->GetSnappedCardsArray())
		{
			if (HandCard->GetCardAbility() != ECardAbility::Decoy)
			{
				bFoundCardToPlay = true;
				PlayCard(HandCard);
				break;
			}
		}

		if (!bFoundCardToPlay)
		{
			GameMode->EndPlayerTurn(PlayerControllerID);
		}
	}
}

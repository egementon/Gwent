// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_Deck.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Row/GW_PlayerHand.h"


AGW_Deck::AGW_Deck()
{
	PrimaryActorTick.bCanEverTick = false;

	CardSpacingX = 3.f;
	bVerticalOffset = true;
}

void AGW_Deck::BeginPlay()
{
	Super::BeginPlay();
}

void AGW_Deck::GiveRandomCardsToHand(int32 StartingHandSize)
{
	AGW_PlayerHand* PlayerHand;
	if (GetPlayerID() == EPlayerID::Player1)
	{
		PlayerHand = GameMode->PlayerHandP1;
	}
	else
	{
		PlayerHand = GameMode->PlayerHandP2;
	}
	
	for (int i = 0; i < StartingHandSize; ++i)
	{
		const int32 RandomIndex = FMath::RandRange(0, SnappedCardsArray.Num() - 1);
		AGW_CardBase* RandomCard = SnappedCardsArray[RandomIndex];
		RandomCard->DetachFromOwnerRow();
		RandomCard->SetOwnerRow(PlayerHand,false);
		RandomCard->SetIsSelectable(true);
	}
	
	GameMode->SetPlayerHandSize(PlayerID, StartingHandSize);
}

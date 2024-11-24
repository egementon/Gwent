// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_PlayerHand.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"


AGW_PlayerHand::AGW_PlayerHand()
{
}

void AGW_PlayerHand::BeginPlay()
{
	Super::BeginPlay();
}

void AGW_PlayerHand::UpdateCardsLocations()
{
	Super::UpdateCardsLocations();
	
	GameMode->SetPlayerHandSize(this->PlayerID, SnappedCardsArray.Num());
}

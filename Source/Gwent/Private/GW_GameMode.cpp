// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "Row/GW_PlayerHand.h"


void AGW_GameMode::SetGraveyard(AGW_Graveyard* NewGraveyard)
{
	Graveyard = NewGraveyard;
	if (PlayerHand)
	{
		PlayerHand->GenerateRandomCards();
	}
}

void AGW_GameMode::BeginPlay()
{
	Super::BeginPlay();
}
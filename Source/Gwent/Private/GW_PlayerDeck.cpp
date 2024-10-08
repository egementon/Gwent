// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_PlayerDeck.h"

#include "Card/GW_CardBase.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"


AGW_PlayerDeck::AGW_PlayerDeck()
{
	bIsPlayerDeck = true;

	TotalPowerText->SetVisibility(false);
}


void AGW_PlayerDeck::GenerateRandomCards()
{
	for (int i = 0; i < 10; ++i)
	{
		auto Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->CardPower = UKismetMathLibrary::RandomInteger(11);
		Card->SetNewOwnerRow(this);
		Card->FinishSpawning(FTransform::Identity);
		SnappedCardsArray.AddUnique(Card);
	}

	UpdateCardsLocations();
}

void AGW_PlayerDeck::BeginPlay()
{
	GenerateRandomCards();
}



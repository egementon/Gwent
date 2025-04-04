﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityScorch.h"

#include "GW_GameMode.h"
#include "Row/GW_UnitRow.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityScorch::ActivateAbility(AGW_CardBase* Card)
{
	int32 HighestCardPower = 0;
	TArray<AGW_CardBase*> HighestCardsArray;

	TArray<AGW_UnitRow*> RowArray = Cast<AGW_GameMode>(Card->GetWorld()->GetAuthGameMode())->AllRowsArray;
	for (int i = 0; i < RowArray.Num(); ++i)
	{
		TArray<AGW_CardBase*> RowCards = RowArray[i]->GetSnappedCardsArray();
		for (AGW_CardBase* RowCard : RowCards)
		{
			if (RowCard == Card || RowCard->bIsHero) continue; // exclude self and hero cards
			
			if (RowCard->GetCardPower() > HighestCardPower)
			{
				HighestCardPower = RowCard->GetCardPower();
				HighestCardsArray.Empty();
				HighestCardsArray.Add(RowCard);
			}
			else if (RowCard->GetCardPower() == HighestCardPower)
			{
				HighestCardsArray.Add(RowCard);
			}
		}
	}

	for (AGW_CardBase* HighCard : HighestCardsArray)
	{
		HighCard->DestroySelfAfterDelay(.6f);
	}
	
	Card->DestroySelfAfterDelay(.15f);
}

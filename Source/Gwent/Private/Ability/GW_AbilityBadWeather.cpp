// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Ability\GW_AbilityBadWeather.h"

#include "GW_Row.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityBadWeather::ActivateAbility(AGW_CardBase* Card)
{
	// TODO: should affect enemy row as well
	TArray<AGW_CardBase*> RowCards = Card->GetOwnerRow()->GetSnappedCardsArray();

	for (AGW_CardBase* RowCard : RowCards)
	{
		RowCard->SetCardPower(1);
	}
	
	Super::ActivateAbility(Card);
}

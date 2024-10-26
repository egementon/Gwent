// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityHorn.h"

#include "GW_Row.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityHorn::ActivateAbility(AGW_CardBase* Card)
{
	TArray<AGW_CardBase*> RowCards = Card->GetOwnerRow()->GetSnappedCardsArray();

	for (AGW_CardBase* RowCard : RowCards)
	{
		RowCard->SetCardPower(RowCard->GetCardPower() * 2);
	}
	
	Super::ActivateAbility(Card);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityTightBond.h"

#include "GW_Row.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityTightBond::ActivateAbility(AGW_CardBase* Card)
{
	TArray<AGW_CardBase*> RowCards = Card->GetOwnerRow()->GetSnappedCardsArray();
	TArray<AGW_CardBase*> SameNameRowCards;
	
	for (AGW_CardBase* RowCard : RowCards)
	{
		if (Card->GetCardName() == RowCard->GetCardName())
		{
			SameNameRowCards.Add(RowCard);
		}
	}

	if (SameNameRowCards.Num() > 0)
	{
		for (AGW_CardBase* SameNameRowCard : SameNameRowCards)
		{
			SameNameRowCard->SetCardPower(SameNameRowCard->GetBaseCardPower() * SameNameRowCards.Num());
		}
	}

	Super::ActivateAbility(Card);
}

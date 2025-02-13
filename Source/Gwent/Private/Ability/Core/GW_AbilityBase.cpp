// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Core/GW_AbilityBase.h"

#include "Card/GW_CardBase.h"

void UGW_AbilityBase::ActivateAbility(AGW_CardBase* Card)
{
	OwnerCard = Card;
}

void UGW_AbilityBase::EndAbility()
{
	if (OwnerCard)
	{
		OwnerCard->EndCardAbility();
	}	
}

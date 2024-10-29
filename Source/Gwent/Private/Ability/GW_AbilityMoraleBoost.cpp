// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityMoraleBoost.h"

#include "Card/GW_CardBase.h"
#include "GW_Row.h"

void UGW_Ability_MoraleBoost::ActivateAbility(AGW_CardBase* Card)
{
	Card->GetOwnerRow()->RowMoraleBoostAddition++;
}

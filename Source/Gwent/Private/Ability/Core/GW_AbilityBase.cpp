// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Core/GW_AbilityBase.h"

#include "GW_Row.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityBase::ActivateAbility(AGW_CardBase* Card)
{
	// call at the end of ability to update row's total power
	Card->GetOwnerRow()->CalculateTotalPower();
}

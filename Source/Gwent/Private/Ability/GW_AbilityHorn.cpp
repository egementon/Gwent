// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityHorn.h"

#include "Row/GW_UnitRow.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityHorn::ActivateAbility(AGW_CardBase* Card)
{
	Card->GetOwnerUnitRow()->bRowHasHorn = true;
}

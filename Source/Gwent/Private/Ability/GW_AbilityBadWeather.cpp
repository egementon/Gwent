// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Ability\GW_AbilityBadWeather.h"

#include "Row/GW_UnitRow.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityBadWeather::ActivateAbility(AGW_CardBase* Card)
{
	// TODO: should affect enemy row as well
	Card->GetOwnerUnitRow()->bRowHasBadWeather = true;
}

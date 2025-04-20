// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityBadWeather.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Row/GW_UnitRow.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityBadWeather::ActivateAbility(AGW_CardBase* Card)
{
	const EUnitRowType CardRowType = Card->GetCardRowType();
	
	TArray<AGW_UnitRow*> RowArray = UGW_FuncLib::GetGameMode(Card->GetWorld())->AllRowsArray;
	for (AGW_UnitRow* Row : RowArray)
	{
		// for example, if row type of the card is selected as melee, bad weather will affect only melee rows
		if (CardRowType == Row->RowType)
		{
			Row->SetRowHasBadWeather(true);
			Row->UpdateAllCardsPowers();
		}
	}
}

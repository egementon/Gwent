// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityTightBond.h"

#include "Row/GW_UnitRow.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityTightBond::ActivateAbility(AGW_CardBase* Card)
{
	// Add card to the tight-bonded array for its name
	TArray<AGW_CardBase*>& BondedArray = Card->GetOwnerUnitRow()->TightBondedCards.FindOrAdd(Card->GetCardName());
	BondedArray.Add(Card);
}

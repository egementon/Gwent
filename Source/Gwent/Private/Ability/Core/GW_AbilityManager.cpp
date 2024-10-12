// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Core/GW_AbilityManager.h"

#include "Ability/GW_AbilityMoraleBoost.h"
#include "Ability/GW_AbilityTightBond.h"
#include "Data/GW_CardDataAsset.h"

UGW_AbilityManager* UGW_AbilityManager::SingletonInstance = nullptr;

UGW_AbilityManager::UGW_AbilityManager()
{
	// Initialization if needed
}

UGW_AbilityManager* UGW_AbilityManager::Get()
{
	if (!SingletonInstance)
	{
		SingletonInstance = NewObject<UGW_AbilityManager>();
	}
	return SingletonInstance;
}

UGW_AbilityBase* UGW_AbilityManager::GetAbility(ECardAbility AbilityType)
{
	switch (AbilityType)
	{
		case ECardAbility::MoraleBoost:
			return NewObject<UGW_Ability_MoraleBoost>();
		case ECardAbility::TightBond:
			return NewObject<UGW_AbilityTightBond>();
		default:
			return nullptr;
	}
	return nullptr;
}

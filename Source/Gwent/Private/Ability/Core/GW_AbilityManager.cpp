// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Core/GW_AbilityManager.h"

#include "Ability/GW_AbilityClearWeather.h"
#include "Ability/GW_AbilityHorn.h"
#include "Ability\GW_AbilityBadWeather.h"
#include "Ability/GW_AbilityMoraleBoost.h"
#include "Ability/GW_AbilityMuster.h"
#include "Ability/GW_AbilityScorch.h"
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
		case ECardAbility::Muster:
			return NewObject<UGW_AbilityMuster>();
		case ECardAbility::BadWeather:
			return NewObject<UGW_AbilityBadWeather>();
		case ECardAbility::ClearWeather:
			return NewObject<UGW_AbilityClearWeather>();
		case ECardAbility::Scorch:
			return NewObject<UGW_AbilityScorch>();
		case ECardAbility::Horn:
			return NewObject<UGW_AbilityHorn>();
		default:
			return nullptr;
	}
}

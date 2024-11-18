// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityClearWeather.h"

#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "GameFramework/GameModeBase.h"
#include "Row/GW_WeatherRow.h"

void UGW_AbilityClearWeather::ActivateAbility(AGW_CardBase* Card)
{
	Cast<AGW_GameMode>(Card->GetWorld()->GetAuthGameMode())->WeatherRow->DestroyBadWeatherCards();
	
	// destroy Clear Weather card
	Card->DestroySelfAfterDelay();
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Row/GW_UnitRow.h"
#include "GW_WeatherRow.generated.h"

UCLASS()
class GWENT_API AGW_WeatherRow : public AGW_UnitRow
{
	GENERATED_BODY()

public:
	AGW_WeatherRow();

	void DestroyBadWeatherCards();
	void ClearWeather();

protected:
	virtual void BeginPlay() override;
	virtual void AddToCardsArray(AGW_CardBase* AddedCard) override;
	
};

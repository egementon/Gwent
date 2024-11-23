﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "GameFramework/Actor.h"
#include "GW_AIController.generated.h"

class AGW_WeatherRow;
class AGW_UnitRow;
class AGW_PlayerHand;

UCLASS()
class GWENT_API AGW_AIController : public AActor
{
	GENERATED_BODY()

public:
	AGW_AIController();
	
	void StartTurn();
	void PlayRandomCard();

	float WaitDuration = 2.f;
	
protected:
	virtual void BeginPlay() override;

	// AI is always Player2
	EPlayerID PlayerControllerID = EPlayerID::Player2;

	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHandP2;
	UPROPERTY() TArray<AGW_UnitRow*> RowArrayP2;
	UPROPERTY() TObjectPtr<AGW_WeatherRow> WeatherRow;

private:
	FTimerHandle WaitBeforePlayTimer;
	
};

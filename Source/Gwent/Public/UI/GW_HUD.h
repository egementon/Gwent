﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GW_HUD.generated.h"

class UGW_PlayerData;
class UGW_GameHUDWidget;
/**
 * 
 */
UCLASS()
class GWENT_API AGW_HUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdatePlayerData(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID);
	
protected:
	virtual void BeginPlay() override;
	
	// Pointer to the main UI widget
	UPROPERTY() UGW_GameHUDWidget* GameHUDWidget;

	// Reference to the widget's class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameHUDWidgetClass;
	
};

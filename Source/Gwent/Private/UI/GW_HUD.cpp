// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GW_HUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/GW_GameHUDWidget.h"



void AGW_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (GameHUDWidgetClass)
	{
		GameHUDWidget = CreateWidget<UGW_GameHUDWidget>(GetWorld(), GameHUDWidgetClass);
		if (GameHUDWidget)
		{
			GameHUDWidget->AddToViewport();
			OnHUDReady.Broadcast(this);
		}
	}
}


void AGW_HUD::UpdatePlayerData(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID)
{
	if (GameHUDWidget)
	{
		GameHUDWidget->UpdateUI(UpdatedPlayerData, PlayerID);
	}
}

UGW_GameHUDWidget* AGW_HUD::GetGameHUDWidget()
{
	return GameHUDWidget;
}

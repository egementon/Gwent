// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GW_GameHUDWidget.h"
#include "Data/GW_PlayerData.h"
#include "UI/W_PlayerData.h"


void UGW_GameHUDWidget::UpdateUI(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID)
{
	if (!UpdatedPlayerData) return;

	if (PlayerID == 1)
	{
		Player1DataWidget->UpdateUI(UpdatedPlayerData);
	}
	else if (PlayerID == 2)
	{
		Player2DataWidget->UpdateUI(UpdatedPlayerData);
	}
}

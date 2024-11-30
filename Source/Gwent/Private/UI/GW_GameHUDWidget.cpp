// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GW_GameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Data/GW_PlayerData.h"


void UGW_GameHUDWidget::UpdateUI(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID)
{
	if (!UpdatedPlayerData) return;

	if (PlayerID == 1)
	{
		Player1ScoreText->SetText(FText::AsNumber(UpdatedPlayerData->GetScore()));
		Player1HandSizeText->SetText(FText::AsNumber(UpdatedPlayerData->GetHandSize()));
		Player1LivesText->SetText(FText::AsNumber(UpdatedPlayerData->GetLifeLeft()));
	}
	else if (PlayerID == 2)
	{
		Player2ScoreText->SetText(FText::AsNumber(UpdatedPlayerData->GetScore()));
		Player2HandSizeText->SetText(FText::AsNumber(UpdatedPlayerData->GetHandSize()));
		Player2LivesText->SetText(FText::AsNumber(UpdatedPlayerData->GetLifeLeft()));
	}
}

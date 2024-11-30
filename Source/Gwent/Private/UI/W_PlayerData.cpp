// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_PlayerData.h"

#include "Components/TextBlock.h"
#include "Data/GW_PlayerData.h"
#include "UI/W_LifeCount.h"


void UW_PlayerData::NativeConstruct()
{
	Super::NativePreConstruct();
	PlayerNameText->SetText(PlayerName);
	PlayerIsPassedText->SetVisibility(ESlateVisibility::Collapsed);
}

void UW_PlayerData::UpdateUI(UGW_PlayerData* UpdatedPlayerData)
{
	if (!UpdatedPlayerData) return;

	PlayerScoreText->SetText(FText::AsNumber(UpdatedPlayerData->GetScore()));
	PlayerHandSizeText->SetText(FText::AsNumber(UpdatedPlayerData->GetHandSize()));
	PlayerLivesWidget->SetLifeCount(UpdatedPlayerData->GetLifeLeft());

	UpdatedPlayerData->IsTurnPassed() ? PlayerIsPassedText->SetVisibility(ESlateVisibility::Visible)
		: PlayerIsPassedText->SetVisibility(ESlateVisibility::Collapsed);
}

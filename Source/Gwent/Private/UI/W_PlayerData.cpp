// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_PlayerData.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/GW_PlayerData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/W_LifeCount.h"


void UW_PlayerData::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerNameText->SetText(PlayerName);
	PlayerIsPassedText->SetVisibility(ESlateVisibility::Collapsed);
}

void UW_PlayerData::UpdateUI(UGW_PlayerData* UpdatedPlayerData)
{
	if (!UpdatedPlayerData) return;

	if (!bInitialized)
	{
		InitializePlayerData(UpdatedPlayerData);
		bInitialized = true;
	}

	PlayerScoreText->SetText(FText::AsNumber(UpdatedPlayerData->GetScore()));
	PlayerHandSizeText->SetText(FText::AsNumber(UpdatedPlayerData->GetHandSize()));
	PlayerLivesWidget->SetLifeCount(UpdatedPlayerData->GetLifeLeft());

	UpdatedPlayerData->IsTurnPassed() ? PlayerIsPassedText->SetVisibility(ESlateVisibility::Visible)
		: PlayerIsPassedText->SetVisibility(ESlateVisibility::Collapsed);

	if (HighlightBorder)
	{
		if (UpdatedPlayerData->IsPlayerTurn())
		{
			HighlightBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			HighlightBorder->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UW_PlayerData::InitializePlayerData(const UGW_PlayerData* InitializedPlayerData) const
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromName(InitializedPlayerData->Data.PlayerName));
	}
	
	if (PlayerImage)
	{
		if (InitializedPlayerData->Data.PlayerAvatar)
		{
			PlayerImage->SetBrushFromTexture(InitializedPlayerData->Data.PlayerAvatar);
		}
	}
}

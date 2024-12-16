// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_PlayerData.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/GW_PlayerData.h"
#include "UI/W_LifeCount.h"


void UW_PlayerData::NativeConstruct()
{
	Super::NativePreConstruct();
	PlayerNameText->SetText(PlayerName);
	PlayerIsPassedText->SetVisibility(ESlateVisibility::Collapsed);
	SetRandomAvatar();
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

void UW_PlayerData::SetRandomAvatar()
{
	// Get reference to the GameMode and PlayerAvatarMap
	AGW_GameMode* GameMode = UGW_FuncLib::GetGameMode(GetWorld());
	if (!GameMode || GameMode->PlayerAvatarMap.Num() == 0)
	{
		return;
	}

	TMap<FName, UTexture2D*>& PlayerDataMap = GameMode->PlayerAvatarMap;

	TArray<FName> Keys;
	PlayerDataMap.GetKeys(Keys);
	int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
	FName RandomKey = Keys[RandomIndex];

	PlayerName = FText::FromName(RandomKey);
	if (PlayerNameText)
	{
		PlayerNameText->SetText(PlayerName);
	}

	if (PlayerImage)
	{
		UTexture2D* SelectedTexture = PlayerDataMap.FindRef(RandomKey);
		if (SelectedTexture)
		{
			PlayerImage->SetBrushFromTexture(SelectedTexture);
		}
	}

	// Remove selected key from the map to ensure it won't be reused
	PlayerDataMap.Remove(RandomKey);
}

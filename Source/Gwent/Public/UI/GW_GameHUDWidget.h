// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GW_GameHUDWidget.generated.h"

class UW_PassProgressBar;
class UW_WinLoseScreen;
class UW_AnnounceState;
class UW_PlayerData;
class UW_LifeCount;
class UImage;
class UTextBlock;
class UGW_PlayerData;
/**
 * 
 */
UCLASS()
class GWENT_API UGW_GameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateUI(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID);

protected:
	void SetPassProgressBarVisibility(UGW_PlayerData* UpdatedPlayerData);
	
	UPROPERTY(meta = (BindWidget))
	UW_PlayerData* Player1DataWidget;

	UPROPERTY(meta = (BindWidget))
	UW_PlayerData* Player2DataWidget;

	UPROPERTY(meta = (BindWidget))
	UW_AnnounceState* AnnounceStateWidget;

	UPROPERTY(meta = (BindWidget))
	UW_WinLoseScreen* WinLoseScreenWidget;

	UPROPERTY(meta = (BindWidget))
	UW_PassProgressBar* PassProgressBarWidget;
	
};

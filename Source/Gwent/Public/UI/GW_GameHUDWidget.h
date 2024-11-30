// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GW_GameHUDWidget.generated.h"

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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1HandSizeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2HandSizeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1LivesText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2LivesText;
	
};

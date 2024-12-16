// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PlayerData.generated.h"

class UImage;
class UGW_PlayerData;
class UW_LifeCount;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GWENT_API UW_PlayerData : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void UpdateUI(UGW_PlayerData* UpdatedPlayerData);

	void SetRandomAvatar();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerHandSizeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerIsPassedText;

	UPROPERTY(meta = (BindWidget))
	UW_LifeCount* PlayerLivesWidget;
	
	UPROPERTY(EditAnywhere)
	FText PlayerName = FText::FromString("Geralt");

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerImage;
	
};

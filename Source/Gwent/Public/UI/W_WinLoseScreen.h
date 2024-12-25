// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "Blueprint/UserWidget.h"
#include "W_WinLoseScreen.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GWENT_API UW_WinLoseScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowWidget(EMatchResult FinalMatchResult, TMap<FName, TArray<int32>> RoundScoreData);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Player1Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Player2Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round1Player1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round2Player1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round3Player1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round1Player2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round2Player2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Round3Player2;

	UPROPERTY(meta = (BindWidget))
	UImage* WinLoseImage;

	UPROPERTY(EditDefaultsOnly)
	FSlateColor WinnerScoreColor;

	// Images
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* VictoryImage;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* DefeatImage;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* DrawImage;
};

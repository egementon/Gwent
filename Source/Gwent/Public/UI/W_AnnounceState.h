// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_AnnounceState.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class GWENT_API UW_AnnounceState : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void SetAnnouncementText(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWidget(); // play animation and hide widget when finished

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnnounceText;

};

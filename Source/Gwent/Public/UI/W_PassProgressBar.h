// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PassProgressBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class GWENT_API UW_PassProgressBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void OnHoldPass(bool bHoldStarted);
	void StartFill();
	void StopFill();
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar;

private:
	FTimerHandle FillTimerHandle;
};

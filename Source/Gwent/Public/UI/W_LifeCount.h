// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_LifeCount.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class GWENT_API UW_LifeCount : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLifeCount(int32 LifeLeft);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* LifeImage1;

	UPROPERTY(meta = (BindWidget))
	UImage* LifeImage2;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FillImage;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* EmptyImage;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_LifeCount.h"

#include "Components/Image.h"

void UW_LifeCount::SetLifeCount(int32 LifeLeft)
{
	TArray LifeImages = { LifeImage1, LifeImage2 };

	for (int32 i = 0; i < LifeImages.Num(); i++)
	{
		LifeImages[i]->SetBrushFromTexture(i < LifeLeft ? FillImage : EmptyImage);
	}
}

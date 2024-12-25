// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_PassProgressBar.h"

#include "GW_PlayerController.h"
#include "Components/ProgressBar.h"

void UW_PassProgressBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
	Cast<AGW_PlayerController>(GetWorld()->GetFirstPlayerController())->OnHoldPassTurn.AddUObject(this, &UW_PassProgressBar::OnHoldPass);
}

void UW_PassProgressBar::OnHoldPass(bool bHoldStarted)
{
	if (bHoldStarted)
	{
		GetWorld()->GetTimerManager().SetTimer(FillTimerHandle, this, &UW_PassProgressBar::StartFill, 0.01f, true);
	}
	else
	{
		StopFill();
	}
}

void UW_PassProgressBar::StartFill()
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(ProgressBar->GetPercent() + 0.01f);
		if (ProgressBar->GetPercent() >= 1.0f)
		{
			SetVisibility(ESlateVisibility::Collapsed);
			StopFill();
		}
	}
}

void UW_PassProgressBar::StopFill()
{
	GetWorld()->GetTimerManager().ClearTimer(FillTimerHandle);
	ProgressBar->SetPercent(0.f);
}


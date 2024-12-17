// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_AnnounceState.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Components/TextBlock.h"

void UW_AnnounceState::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);

	// bind to announcements
	if (AGW_GameMode* GameMode = UGW_FuncLib::GetGameMode(GetWorld()))
	{
		GameMode->OnAnnouncementMessage.AddLambda([this](const FString& Message)
		{
			SetAnnouncementText(Message);
		});
	}
}

void UW_AnnounceState::SetAnnouncementText(const FString& Message)
{
	AnnounceText->SetText(FText::FromString(Message));
	ShowWidget();
}

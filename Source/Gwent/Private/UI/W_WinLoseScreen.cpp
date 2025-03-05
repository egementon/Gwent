// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_WinLoseScreen.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UW_WinLoseScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Collapsed);

	UGW_FuncLib::GetGameMode(GetWorld())->OnMatchEnd.AddUObject(this, &UW_WinLoseScreen::ShowWidget);
}

void UW_WinLoseScreen::ShowWidget(EMatchResult FinalMatchResult, TMap<FName, TArray<int32>> RoundScoreData)
{
	if (RoundScoreData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("RoundScoreData is empty."));
		return;
	}

	auto UpdateTextBlock = [this](UTextBlock* TextBlock, int32 Score, bool bIsWinner)
	{
		if (TextBlock)
		{
			TextBlock->SetText(FText::FromString(FString::FromInt(Score)));
			
			if (bIsWinner)
			{
				TextBlock->SetColorAndOpacity(WinnerScoreColor);
			}
		}
	};

	// Player Scores
	TArray<TArray<int32>> PlayerScores;
	RoundScoreData.GenerateValueArray(PlayerScores);
	const TArray<int32>& Player1Scores = PlayerScores[0]; // First player's scores
	const TArray<int32>& Player2Scores = PlayerScores[1]; // Second player's scores

	for (int32 RoundIndex = 0; RoundIndex < 3; ++RoundIndex)
	{
		bool bIsP1Winner = false;
		bool bIsP2Winner = false;

		const int32 P1Score = Player1Scores[RoundIndex];
		const int32 P2Score = Player2Scores[RoundIndex];
		
		if (P1Score > P2Score)
		{
			bIsP1Winner = true; // Player 1 wins this round
		}
		else if (P2Score > P1Score)
		{
			bIsP2Winner = true; // Player 2 wins this round
		}
		
		if (RoundIndex == 0)
		{
			UpdateTextBlock(Text_Round1Player1, P1Score, bIsP1Winner);
			UpdateTextBlock(Text_Round1Player2, P2Score, bIsP2Winner);
		}
		else if (RoundIndex == 1)
		{
			UpdateTextBlock(Text_Round2Player1, P1Score, bIsP1Winner);
			UpdateTextBlock(Text_Round2Player2, P2Score, bIsP2Winner);
		}
		else if (RoundIndex == 2)
		{
			UpdateTextBlock(Text_Round3Player1, P1Score, bIsP1Winner);
			UpdateTextBlock(Text_Round3Player2, P2Score, bIsP2Winner);
		}
	}

	// Player Names
	TArray<FName> PlayerNames;
	RoundScoreData.GenerateKeyArray(PlayerNames);
	if (Text_Player1Name && Text_Player2Name)
	{
		Text_Player1Name->SetText(FText::FromName(PlayerNames[0]));
		Text_Player2Name->SetText(FText::FromName(PlayerNames[1]));
	}

	SetVisibility(ESlateVisibility::Visible);

	switch (FinalMatchResult)
	{
	case EMatchResult::Player1Wins:
		WinLoseImage->SetBrushFromTexture(VictoryImage);
		break;
	
	case EMatchResult::Player2Wins:
		WinLoseImage->SetBrushFromTexture(DefeatImage);
		break;
	
	case EMatchResult::Draw:
		WinLoseImage->SetBrushFromTexture(DrawImage);
		break;
	}

	Button_Replay->OnClicked.AddDynamic(this, &UW_WinLoseScreen::OnReplayButtonClicked);
	Button_MainMenu->OnClicked.AddDynamic(this, &UW_WinLoseScreen::OnMainMenuButtonClicked);
}

void UW_WinLoseScreen::OnReplayButtonClicked()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, *CurrentLevelName);
}

void UW_WinLoseScreen::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

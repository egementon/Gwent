// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/GW_PlayerData.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"

void UGW_PlayerData::SetHandSize(int32 NewHandSize)
{
	if (Data.HandSize != NewHandSize)
	{
		Data.HandSize = NewHandSize;
		OnPlayerDataChanged.Broadcast(this, PlayerID);
	}
}

void UGW_PlayerData::SetScore(int32 NewScore)
{
	if (Data.Score != NewScore)
	{
		Data.Score = NewScore;
		OnPlayerDataChanged.Broadcast(this, PlayerID);
	}
}

void UGW_PlayerData::SetLifeLeft(int32 NewLifeLeft)
{
	if (Data.LifeLeft != NewLifeLeft)
	{
		Data.LifeLeft = NewLifeLeft;
		OnPlayerDataChanged.Broadcast(this, PlayerID);
	}
}

void UGW_PlayerData::SetPassedTurn(bool bNewPassedTurn)
{
	if (Data.PassedTurn != bNewPassedTurn)
	{
		Data.PassedTurn = bNewPassedTurn;
		OnPlayerDataChanged.Broadcast(this, PlayerID);
	}
}

void UGW_PlayerData::SetIsPlayerTurn(bool bNewIsPlayerTurn)
{
	if (Data.IsPlayerTurn != bNewIsPlayerTurn)
	{
		Data.IsPlayerTurn = bNewIsPlayerTurn;
		OnPlayerDataChanged.Broadcast(this, PlayerID);
	}
}

void UGW_PlayerData::ResetDataForNextRound()
{
	Data.Score = 0;
	Data.PassedTurn = false;
	OnPlayerDataChanged.Broadcast(this, PlayerID);
}

void UGW_PlayerData::DecrementLifeLeft()
{
	Data.LifeLeft--;
	OnPlayerDataChanged.Broadcast(this, PlayerID);
}

int32 UGW_PlayerData::GetHandSize() const
{
	return Data.HandSize;
}

int32 UGW_PlayerData::GetScore() const
{
	return Data.Score;
}

int32 UGW_PlayerData::GetLifeLeft() const
{
	return Data.LifeLeft;
}

bool UGW_PlayerData::IsTurnPassed() const
{
	return Data.PassedTurn;
}

bool UGW_PlayerData::IsPlayerTurn() const
{
	return Data.IsPlayerTurn;
}

void UGW_PlayerData::SetRandomNameAndAvatar()
{
	// Get reference to the GameMode and PlayerAvatarMap
	AGW_GameMode* GameMode = UGW_FuncLib::GetGameMode(GetWorld());
	if (!GameMode || GameMode->PlayerAvatarMap.Num() == 0)
	{
		return;
	}

	TMap<FName, UTexture2D*>& PlayerDataMap = GameMode->PlayerAvatarMap;

	TArray<FName> Keys;
	PlayerDataMap.GetKeys(Keys);
	const int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
	const FName RandomKey = Keys[RandomIndex];

	Data.PlayerName = RandomKey;

	if (UTexture2D* SelectedTexture = PlayerDataMap.FindRef(RandomKey))
	{
		Data.PlayerAvatar = SelectedTexture;
	}
	
	// Remove selected key from the map to ensure it won't be reused
	PlayerDataMap.Remove(RandomKey);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/GW_PlayerData.h"

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

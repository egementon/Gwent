// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_FuncLib.h"

#include "GW_GameMode.h"

EPlayerID UGW_FuncLib::GetWhoseTurn(const UWorld* World)
{
	if (AGW_GameMode* GameMode = Cast<AGW_GameMode>(World->GetAuthGameMode()))
	{
		return GameMode->GetWhoseTurn();
	}
	return {};
}

bool UGW_FuncLib::CheckIsPlayerIDTurn(const UWorld* World, const EPlayerID PlayerIDToCheck)
{
	return GetWhoseTurn(World) == PlayerIDToCheck;
}

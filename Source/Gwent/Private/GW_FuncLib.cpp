// Fill out your copyright notice in the Description page of Project Settings.


#include "GW_FuncLib.h"
#include "GW_GameMode.h"


AGW_GameMode* UGW_FuncLib::GetGameMode(const UWorld* World)
{
	if (AGW_GameMode* GameMode = Cast<AGW_GameMode>(World->GetAuthGameMode()))
	{
		return GameMode;
	}
	
	return nullptr;
}

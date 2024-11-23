// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Row/GW_RowBase.h"
#include "GW_FuncLib.generated.h"

class AGW_GameMode;
/**
 * 
 */
UCLASS()
class GWENT_API UGW_FuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static AGW_GameMode* GetGameMode(const UWorld* World);
	
};

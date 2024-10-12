// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GW_AbilityManager.generated.h"

enum class ECardAbility : uint8;
class UGW_AbilityBase;
/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityManager : public UObject
{
	GENERATED_BODY()

public:
	static UGW_AbilityManager* Get();

	UGW_AbilityBase* GetAbility(ECardAbility AbilityType);

private:
	UGW_AbilityManager();
	
	static UGW_AbilityManager* SingletonInstance;
	
};

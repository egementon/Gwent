// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GW_AbilityBase.h"
#include "GW_AbilityHorn.generated.h"

/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityHorn : public UGW_AbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(AGW_CardBase* Card) override;
};

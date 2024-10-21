﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GW_AbilityBase.h"
#include "GW_AbilityScorch.generated.h"

/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityScorch : public UGW_AbilityBase
{
	GENERATED_BODY()

	virtual void ActivateAbility(AGW_CardBase* Card) override;
};

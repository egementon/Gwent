// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GW_AbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(class AGW_CardBase* Card) PURE_VIRTUAL(UGW_AbilityBase::ActivateAbility, );

};

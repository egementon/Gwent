// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GW_AbilityBase.generated.h"

class AGW_CardBase;
/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(AGW_CardBase* Card);

protected:
	virtual void EndAbility();
	
	UPROPERTY() AGW_CardBase* OwnerCard;

};

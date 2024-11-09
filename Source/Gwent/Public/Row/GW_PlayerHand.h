// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Row/GW_UnitRow.h"
#include "GameFramework/Actor.h"
#include "GW_PlayerHand.generated.h"

class UGW_CardDataAsset;
class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_PlayerHand : public AGW_RowBase
{
	GENERATED_BODY()

public:
	AGW_PlayerHand();

protected:
	virtual void BeginPlay() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_RowBase.h"
#include "GW_Graveyard.generated.h"

UCLASS()
class GWENT_API AGW_Graveyard : public AGW_RowBase
{
	GENERATED_BODY()

public:
	AGW_Graveyard();

protected:
	virtual void BeginPlay() override;


};

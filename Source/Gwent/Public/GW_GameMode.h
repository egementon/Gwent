// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GW_GameMode.generated.h"

class AGW_Row;
/**
 * 
 */
UCLASS()
class GWENT_API AGW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	TObjectPtr<AGW_Row> PlayerDeck;
	TArray<AGW_Row*> RowArray;

protected:
	virtual void BeginPlay() override;

};

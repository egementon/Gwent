// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GW_GameMode.generated.h"

class AGW_Graveyard;
class AGW_PlayerHand;
class AGW_UnitRow;
/**
 * 
 */
UCLASS()
class GWENT_API AGW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SetGraveyard(AGW_Graveyard* NewGraveyard);

	UPROPERTY()
	TObjectPtr<AGW_PlayerHand> PlayerHand;

	UPROPERTY()
	TObjectPtr<AGW_Graveyard> Graveyard;
	
	UPROPERTY()
	TArray<AGW_UnitRow*> RowArray;

protected:
	virtual void BeginPlay() override;

};

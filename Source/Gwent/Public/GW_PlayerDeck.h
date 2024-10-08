// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Row.h"
#include "GameFramework/Actor.h"
#include "GW_PlayerDeck.generated.h"

class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_PlayerDeck : public AGW_Row
{
	GENERATED_BODY()

public:
	AGW_PlayerDeck();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGW_CardBase> CardClass; 
	
	void GenerateRandomCards();
	
};

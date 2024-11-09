// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_RowBase.generated.h"

class UBoxComponent;
class AGW_CardBase;

UCLASS()
class GWENT_API AGW_RowBase : public AActor
{
	GENERATED_BODY()

public:
	AGW_RowBase();

	TArray<AGW_CardBase*> GetSnappedCardsArray();

	virtual void AddToCardsArray(AGW_CardBase* AddedCard);
	virtual void RemoveFromCardsArray(AGW_CardBase* RemovedCard);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* RowBoxComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<AGW_CardBase*> SnappedCardsArray;

	void UpdateCardsLocations();

	float CardSpacingX = 107.f; // default for PlayerHand and UnitRows
	float CardSpacingZ = 1.f;
	bool bVerticalOffset = false;// true for Graveyard and Deck
	
};

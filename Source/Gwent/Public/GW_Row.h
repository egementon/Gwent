// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Row.generated.h"

class UTextRenderComponent;
class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_Row : public AActor
{
	GENERATED_BODY()

public:
	AGW_Row();

	void AddToCardsArray(AGW_CardBase* AddedCard);
	void RemoveFromCardsArray(AGW_CardBase* RemovedCard);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* RowBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TotalPowerText;

	UPROPERTY(BlueprintReadOnly)
	TArray<AGW_CardBase*> SnappedCardsArray;

	void UpdateCardsLocations();

	void CalculateTotalPower();
	
	int32 TotalPower;
	
};

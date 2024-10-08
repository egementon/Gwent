// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_PlayerDeck.generated.h"

class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_PlayerDeck : public AActor
{
	GENERATED_BODY()

public:
	AGW_PlayerDeck();
	
	void AddToCardsArray(AGW_CardBase* AddedCard);
	void RemoveFromCardsArray(AGW_CardBase* RemovedCard);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* RowBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGW_CardBase> CardClass; 
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AGW_CardBase*> SnappedCardsArray;
	

	void UpdateCardsLocations();
	
	void GenerateRandomCards();
	
	
};

﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GW_Row.generated.h"

enum class ECardRowType : uint8;
class UTextRenderComponent;
class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_Row : public AActor
{
	GENERATED_BODY()

public:
	AGW_Row();

	// Getters
	TArray<AGW_CardBase*> GetSnappedCardsArray();
	AGW_CardBase* GetSnappedSpecialCard();
	bool IsSpecialSlotEmpty();
	bool IsPlayerDeck();
	// Setters
	void SetSpecialCard(AGW_CardBase* SpecialCard);
	void SetSpecialSlotEmpty(bool bIsEmpty);
	
	void AddToCardsArray(AGW_CardBase* AddedCard);
	void RemoveFromCardsArray(AGW_CardBase* RemovedCard);
	void CalculateRowPower();
	bool IsValidRowForCard(AGW_CardBase* Card);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardRowType RowType;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* RowBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpecialSlotBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TotalPowerText;

	UPROPERTY(BlueprintReadOnly)
	TArray<AGW_CardBase*> SnappedCardsArray;

	UPROPERTY()
	AGW_CardBase* SnappedSpecialCard;

	void UpdateCardsLocations();
	
	int32 TotalPower;

	bool bIsPlayerDeck = false;
	bool bIsSpecialSlotEmpty = true;

	
};

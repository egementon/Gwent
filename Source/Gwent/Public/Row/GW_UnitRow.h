// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Row/GW_RowBase.h"
#include "GW_UnitRow.generated.h"

enum class EUnitRowType : uint8;
class UTextRenderComponent;
class AGW_CardBase;
class UBoxComponent;

UCLASS()
class GWENT_API AGW_UnitRow : public AGW_RowBase
{
	GENERATED_BODY()

public:
	AGW_UnitRow();

	// Getters
	AGW_CardBase* GetSnappedSpecialCard();
	bool IsSpecialSlotEmpty();

	// Setters
	void SetSpecialCard(AGW_CardBase* SpecialCard);
	void SetSpecialSlotEmpty(bool bIsEmpty);
	
	void UpdateAllCardsPowers(); 
	void CalculateRowPower();
	bool IsValidRowForCard(AGW_CardBase* Card);
	//TODO: GetValidRows(); use for AI and highlighting
	
	virtual void AddToCardsArray(AGW_CardBase* AddedCard) override;
	virtual void RemoveFromCardsArray(AGW_CardBase* RemovedCard) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitRowType RowType;
	
	// Card power calculation parameters in row
	bool bRowHasBadWeather = false;
	bool bRowHasHorn = false;
	int32 RowMoraleBoostAddition = 0;
	
	// tight-bonded cards
	TMap<FName, TArray<AGW_CardBase*>> TightBondedCards;
	void OnTightBondedCardRemoved(AGW_CardBase* RemovedCard);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpecialSlotBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TotalPowerText;

	void SetCardPowerParameters(AGW_CardBase* AddedCard);
	
	UPROPERTY() AGW_CardBase* SnappedSpecialCard;
	int32 TotalPower;
	bool bIsSpecialSlotEmpty = true;
	bool bIsWeatherRow = false;
	
};

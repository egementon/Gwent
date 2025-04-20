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
	int32 GetTotalPower();

	// Setters
	void SetSpecialCard(AGW_CardBase* SpecialCard);
	void SetSpecialSlotEmpty(bool bIsEmpty);
	void SetRowHasBadWeather(bool bHasBadWeather);
	
	void UpdateAllCardsPowers(); 
	void CalculateRowPower();
	bool IsValidRowForCard(AGW_CardBase* Card);
	
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

	// Highlight
	void HighlightRow(bool bHighlight);
	void HighlightRowSpecialSlot(bool bHighlight);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpecialSlotBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TotalPowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* AreaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SpecialSlotAreaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BadWeatherAreaMesh;

	void SetCardPowerParameters(AGW_CardBase* Card);
	
	UPROPERTY() AGW_CardBase* SnappedSpecialCard;
	int32 TotalPower;
	bool bIsSpecialSlotEmpty = true;
	bool bIsWeatherRow = false;
	
};

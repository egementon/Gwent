// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MedicAbility.generated.h"

class UScrollBox;
class UGW_AbilityMedic;
class AGW_CardBase;
class UHorizontalBox;
class UImage;
class UButton;

/**
 * 
 */
UCLASS()
class GWENT_API UW_MedicAbility : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMedicAbility(UGW_AbilityMedic* NewMedicAbility);

private:

	virtual void NativeConstruct() override;

	UFUNCTION() void OnSelectedCard(AGW_CardBase* SelectedCard);
	UFUNCTION() void OnCardButtonPressed();
	void CreateCardWidgets();

	TArray<AGW_CardBase*> GraveyardCards;
	UPROPERTY() TMap<UButton*, AGW_CardBase*> CardButtonMap;
	UPROPERTY() UGW_AbilityMedic* MedicAbility;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CardContainer;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CardScrollBox;
	
};

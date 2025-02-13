// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GW_AbilityBase.h"
#include "GW_AbilityMedic.generated.h"

/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilityMedic : public UGW_AbilityBase
{
	GENERATED_BODY()

public:
	void OnCardSelected(AGW_CardBase* SelectedCard);
	
	TArray<AGW_CardBase*> SelectableGraveyardCards;

protected:
	virtual void ActivateAbility(AGW_CardBase* Card) override;

	FTimerHandle DelayTimer;
	
};

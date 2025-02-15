// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GW_AbilityBase.h"
#include "GW_AbilitySpy.generated.h"

class AGW_PlayerHand;
class AGW_Deck;
/**
 * 
 */
UCLASS()
class GWENT_API UGW_AbilitySpy : public UGW_AbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(AGW_CardBase* Card) override;

	void GetRandomCardFromDeckToHand();

	TObjectPtr<AGW_Deck> PlayerDeck;
	TObjectPtr<AGW_PlayerHand> PlayerHand;
};

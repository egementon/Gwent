// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_RowBase.h"
#include "GW_Deck.generated.h"

UCLASS()
class GWENT_API AGW_Deck : public AGW_RowBase
{
	GENERATED_BODY()

public:
	AGW_Deck();

protected:
	virtual void BeginPlay() override;
	void GiveRandomCardsToHand();

private:
	FTimerHandle StartTimer;
	
};

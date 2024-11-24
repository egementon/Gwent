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
	
	void GiveRandomCardsToHand(int32 StartingHandSize);

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle StartTimer;
	
};

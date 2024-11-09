// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GW_GameMode.generated.h"

class AGW_RowBase;
class AGW_Deck;
class AGW_CardBase;
class UGW_CardDataAsset;
class AGW_Graveyard;
class AGW_PlayerHand;
class AGW_UnitRow;
/**
 * 
 */
UCLASS()
class GWENT_API AGW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void RegisterRow(AGW_RowBase* NewRow);

	// Row references
	UPROPERTY() TArray<AGW_UnitRow*> RowArray;
	UPROPERTY() TObjectPtr<AGW_Deck> Deck;
	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHand;
	UPROPERTY() TObjectPtr<AGW_Graveyard> Graveyard;

protected:
	virtual void BeginPlay() override;
	void GenerateRandomCardsForDeck();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGW_CardBase> CardClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGW_CardDataAsset* CardDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DeckSize = 15; 

};

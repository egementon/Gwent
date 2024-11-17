// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
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
	EPlayerID GetWhoseTurn();

	// Row references
	UPROPERTY() TArray<AGW_UnitRow*> RowArrayP1;
	UPROPERTY() TObjectPtr<AGW_Deck> DeckP1;
	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHandP1;
	UPROPERTY() TObjectPtr<AGW_Graveyard> GraveyardP1;

	UPROPERTY() TArray<AGW_UnitRow*> RowArrayP2;
	UPROPERTY() TObjectPtr<AGW_Deck> DeckP2;
	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHandP2;
	UPROPERTY() TObjectPtr<AGW_Graveyard> GraveyardP2;

	UPROPERTY() TArray<AGW_UnitRow*> AllRowsArray;

	
protected:
	virtual void BeginPlay() override;
	void GenerateRandomCardsForDeck();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AGW_CardBase> CardClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGW_CardDataAsset* CardDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 DeckSize = 15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSpawnAIController = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition = "bSpawnAIController"))
	float AIWaitDuration = 2.f;
	
	EPlayerID WhoseTurn = EPlayerID::Player1; // default of this is the starting player

};

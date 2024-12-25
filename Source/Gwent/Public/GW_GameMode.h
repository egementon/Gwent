// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "GameFramework/GameModeBase.h"
#include "UI/GW_HUD.h"
#include "GW_GameMode.generated.h"

class UGW_PlayerData;
class AGW_PlayerController;
class AGW_AIController;
class AGW_WeatherRow;
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
typedef TMap<FName, TArray<int32>> FRoundScoreDataMap;
DECLARE_MULTICAST_DELEGATE(FOnNewRoundStartedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnnouncementMessageSignature, const FString&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMatchEndSignature, EMatchResult, FRoundScoreDataMap);

UCLASS()
class GWENT_API AGW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void RegisterRow(AGW_RowBase* NewRow);

	// Row references
	// Player 1 rows
	UPROPERTY() TArray<AGW_UnitRow*> RowArrayP1;
	UPROPERTY() TObjectPtr<AGW_Deck> DeckP1;
	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHandP1;
	UPROPERTY() TObjectPtr<AGW_Graveyard> GraveyardP1;

	// Player 2 rows
	UPROPERTY() TArray<AGW_UnitRow*> RowArrayP2;
	UPROPERTY() TObjectPtr<AGW_Deck> DeckP2;
	UPROPERTY() TObjectPtr<AGW_PlayerHand> PlayerHandP2;
	UPROPERTY() TObjectPtr<AGW_Graveyard> GraveyardP2;

	// Common rows
	UPROPERTY() TObjectPtr<AGW_WeatherRow> WeatherRow;
	UPROPERTY() TArray<AGW_UnitRow*> AllRowsArray;

	// Turn-based system functions
	bool IsMyTurn(EPlayerID PlayerID) const;
	void EndPlayerTurn(EPlayerID PlayerID);
	void PlayerPassedTurn(EPlayerID PlayerID);

	// PlayerData
	UPROPERTY() UGW_PlayerData* Player1Data;
	UPROPERTY() UGW_PlayerData* Player2Data;

	// RoundScoreData
	TMap<FName, TArray<int32>> RoundScoreData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UTexture2D*> PlayerAvatarMap;
	
	void SetPlayerHandSize(EPlayerID PlayerID, int32 HandSize);
	void UpdatePlayerScore(EPlayerID PlayerID);
	int32 CalculateScore(TArray<AGW_UnitRow*> RowArray);

	FOnNewRoundStartedSignature OnNewRoundStarted;
	FOnAnnouncementMessageSignature OnAnnouncementMessage;
	FOnMatchEndSignature OnMatchEnd;
	
protected:
	virtual void BeginPlay() override;

	void GenerateRandomCardsForDeck();
	void GiveDeckCardsToHands();

	void SetGamePhase(EGamePhase NewPhase);

	void StartPlayerTurn();

	EMatchResult DetermineResult(bool& bMatchEnded);

	void ClearAllRows();

	void OnHUDReady(AGW_HUD* HUD);

	UPROPERTY()
	AGW_PlayerController* PlayerController; // Player 1
	
	UPROPERTY()
	AGW_AIController* AIController; // Player 2
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGW_CardBase> CardClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGW_CardDataAsset* CardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InitialDeckSize = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InitialHandSize = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSpawnAIController = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition = "bSpawnAIController"))
	float AIWaitDuration = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebug_FilterCards = false;
	
	EGamePhase CurrentGamePhase = EGamePhase::Start;
	
	EPlayerID LastPlayedID = EPlayerID::Player2; // ID of the last player who played card
	
	int32 RoundIndex = 0;

	EMatchResult FinalMatchResult = EMatchResult::Draw;


private:
	FTimerHandle WaitPhaseTimer;

};

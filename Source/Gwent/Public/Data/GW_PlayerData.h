// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GW_PlayerData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDataChangedSignature, UGW_PlayerData*, UpdatedPlayerData, int32, PlayerID);

/**
 * 
 */
UCLASS()
class GWENT_API UGW_PlayerData : public UObject
{
	GENERATED_BODY()

public:
	static UGW_PlayerData* CreateWithPlayerID(UObject* Outer, int32 PlayerID)
	{
		UGW_PlayerData* NewData = NewObject<UGW_PlayerData>(Outer);
		NewData->PlayerID = PlayerID;
		NewData->SetRandomNameAndAvatar();
		return NewData;
	}

	struct FPlayerData
	{
		int32 HandSize = 0; // number of cards in hand currently
		int32 Score = 0; // sum of 3 rows powers
		int32 LifeLeft = 2; // players start with 2 lives
		bool PassedTurn = false;
		bool IsPlayerTurn = false;

		// for widgets
		FName PlayerName = "Geralt";
		UTexture2D* PlayerAvatar = nullptr;
	};
	
	FPlayerData Data;
	FOnPlayerDataChangedSignature OnPlayerDataChanged;
	
	// Setters
	void SetHandSize(int32 NewHandSize);
	void SetScore(int32 NewScore);
	void SetLifeLeft(int32 NewLifeLeft);
	void SetPassedTurn(bool bNewPassedTurn);
	void SetIsPlayerTurn(bool bNewIsPlayerTurn);
	
	void ResetDataForNextRound();
	void DecrementLifeLeft();

	// Getters
	int32 GetHandSize() const;
	int32 GetScore() const;
	int32 GetLifeLeft() const;
	bool IsTurnPassed() const;
	bool IsPlayerTurn() const;

protected:
	int32 PlayerID = 0;
	
	void SetRandomNameAndAvatar();

};

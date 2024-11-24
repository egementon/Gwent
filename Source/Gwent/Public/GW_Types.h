#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerID : uint8
{
	Player1   UMETA(DisplayName = "Player 1"),
	Player2   UMETA(DisplayName = "Player 2"),
};

enum class EGamePhase
{
	Start,
	Player1Turn,
	Wait,
	Player2Turn,
	End
};

struct FPlayerData
{
	int32 HandSize = 0; // number of cards in hand currently
	int32 Score = 0; // sum of 3 rows powers
	int32 LifeCount = 2; // players start with 2 lives
	bool PassedTurn = false;
	
	void ResetDataForNextRound()
	{
		Score = 0;
		PassedTurn = false;
	}
};

UENUM(BlueprintType)
enum class EMatchResult : uint8
{
	Player1Wins UMETA(DisplayName = "Player 1 Wins"),
	Player2Wins UMETA(DisplayName = "Player 2 Wins"),
	Draw UMETA(DisplayName = "Draw")
};


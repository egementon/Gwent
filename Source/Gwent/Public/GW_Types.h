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
	RoundEnd,
	MatchEnd
};

UENUM(BlueprintType)
enum class EMatchResult : uint8
{
	Player1Wins UMETA(DisplayName = "Player 1 Wins"),
	Player2Wins UMETA(DisplayName = "Player 2 Wins"),
	Draw UMETA(DisplayName = "Draw")
};


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

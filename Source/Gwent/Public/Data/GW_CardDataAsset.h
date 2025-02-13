// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GW_CardDataAsset.generated.h"


UENUM(BlueprintType)
enum class EUnitRowType : uint8
{
	// can be placed only on Melee row.
	Melee   UMETA(DisplayName = "Melee"),
	// can be placed only on Ranged row.
	Ranged  UMETA(DisplayName = "Ranged"),
	// can be placed only on Siege row.
	Siege   UMETA(DisplayName = "Siege"),
	// can be placed on Melee or Ranged rows.
	Agile   UMETA(DisplayName = "Agile"),
	// can be placed only on any row.
	AllRows UMETA(DisplayName = "All Rows")
};

UENUM(BlueprintType)
enum class ECardAbility : uint8
{
	// standard card
	NoAbility    UMETA(DisplayName = "No Ability"),
	// when placed next to a card with the same name, doubles the strength of both (or more) cards.
	TightBond    UMETA(DisplayName = "Tight Bond"),
	// add +1 strength to all units in the row in which they are played, excluding themselves.
	MoraleBoost  UMETA(DisplayName = "Morale Boost"),
	// when played, automatically find any cards with the same name in your deck and play them instantly.
	Muster       UMETA(DisplayName = "Muster"),
	// allows you to choose one card from your graveyard and play it instantly (no Heroes or Special Cards)
	Medic        UMETA(DisplayName = "Medic"),
	// reduces the strength of all cards of a given row on the battlefield, including your own.
	BadWeather   UMETA(DisplayName = "Bad Weather"),
	// removes all Weather Card effects. 
	ClearWeather UMETA(DisplayName = "Clear Weather"),
	// destroys the strongest card(s) on the battlefield 
	Scorch       UMETA(DisplayName = "Scorch"),
	// double the strength of the selected row (except if another horn is already being used on the row).
	Horn         UMETA(DisplayName = "Horn")
};


USTRUCT(BlueprintType)
struct FCardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int32 Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	EUnitRowType RowType = EUnitRowType::Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bIsSpecial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bIsHero = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardAbility Ability = ECardAbility::NoAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	USoundBase* SFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bDebug_ExcludeFromDeck = false;

};


/**
 * 
 */
UCLASS()
class GWENT_API UGW_CardDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCardData> CardDataArray;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GW_CardDataAsset.generated.h"


UENUM(BlueprintType)
enum class ECardRowType : uint8
{
	Melee   UMETA(DisplayName = "Melee"),
	Ranged  UMETA(DisplayName = "Ranged"),
	Siege   UMETA(DisplayName = "Siege"),
	Agile   UMETA(DisplayName = "Agile"), // Melee and Ranged
	AllRows UMETA(DisplayName = "All Rows")
};

UENUM(BlueprintType)
enum class ECardAbility : uint8
{
	NoAbility    UMETA(DisplayName = "No Ability"),
	TightBond    UMETA(DisplayName = "Tight Bond"),
	MoraleBoost  UMETA(DisplayName = "Morale Boost"),
	Muster       UMETA(DisplayName = "Muster"),
	BadWeather   UMETA(DisplayName = "Bad Weather"),
	ClearWeather UMETA(DisplayName = "Clear Weather"),
	Scorch       UMETA(DisplayName = "Scorch")
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
	ECardRowType RowType = ECardRowType::Melee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bIsSpecial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardAbility Ability = ECardAbility::NoAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTexture2D* Image = nullptr;

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

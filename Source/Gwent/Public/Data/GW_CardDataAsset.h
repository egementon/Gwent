// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GW_CardDataAsset.generated.h"


UENUM(BlueprintType)
enum class ECardRowType : uint8
{
	Melee  UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	Siege  UMETA(DisplayName = "Siege")
};

UENUM(BlueprintType)
enum class ECardAbility : uint8
{
	NoAbility      UMETA(DisplayName = "No Ability"),
	TightBond    UMETA(DisplayName = "Tight Bond"),
	MoraleBoost  UMETA(DisplayName = "Morale Boost")
};


USTRUCT(BlueprintType)
struct FCardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int32 Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardRowType RowType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	ECardAbility Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UMaterial* ColorMaterial;
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

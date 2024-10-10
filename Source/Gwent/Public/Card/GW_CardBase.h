// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GW_CardDataAsset.h"
#include "GameFramework/Actor.h"
#include "GW_CardBase.generated.h"

class UTextRenderComponent;
class AGW_Row;

UCLASS()
class GWENT_API AGW_CardBase : public AActor
{
	GENERATED_BODY()

public:
	AGW_CardBase();

	// void UpdateCardLocation(const FVector& NewLocation);
	// void SetMouseCollision(bool bIsActive);

	// Row Functions
	void SetNewOwnerRow(AGW_Row* NewOwner);
	void DetachFromOwnerRow();

	bool bIsSnapped;

	void InitializeCardData(FCardData NewCardData);
	
	// Card Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FName CardName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	int32 CardPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	ECardRowType CardRowType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	ECardAbility CardAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	UTexture2D* CardIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	UMaterial* ColorMaterial;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* CardPowerText;

	UPROPERTY()
	AGW_Row* OwnerRow;

private:
	// for editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	


};

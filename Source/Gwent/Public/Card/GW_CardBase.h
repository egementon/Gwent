﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	// Getters
	AGW_Row* GetOwnerRow() const;
	FName GetCardName() const;
	int32 GetCardPower() const;
	ECardRowType GetCardRowType() const;
	ECardAbility GetCardAbility() const;
	int32 GetBaseCardPower() const;
	
	void SetCardPower(int32 NewCardPower);

	// Row Functions
	void SetOwnerRow(AGW_Row* NewOwner, bool bShouldActivateAbility);
	void DetachFromOwnerRow();
	void SetOwnerRowAsPlayerDeck();

	bool bIsSnapped = false;
	bool bIsSpecial = false; // placed on the special slot on the row
	bool bIsHero = false; // can not be affected from buff/debuffs, i.e. immune card

	void InitializeCardData(FCardData NewCardData);
	void DestroySelf();
	void DestroySelfAfterDelay(float Delay = 1.f);
	
protected:
	virtual void BeginPlay() override;

	void CanActivateAbility();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* CardPowerText;

	UPROPERTY()
	AGW_Row* OwnerRow;
	
	int32 BaseCardPower;
	
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
	UTexture2D* CardImage;
	
private:
	// for editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FTimerHandle DestroyTimer;
	

};

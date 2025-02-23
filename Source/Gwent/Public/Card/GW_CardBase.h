// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "Data/GW_CardDataAsset.h"
#include "GameFramework/Actor.h"
#include "GW_CardBase.generated.h"

class AGW_Graveyard;
class AGW_UnitRow;
class UTextRenderComponent;
class AGW_RowBase;

DECLARE_MULTICAST_DELEGATE(FOnCardAbilityEndedSignature);

UCLASS()
class GWENT_API AGW_CardBase : public AActor
{
	GENERATED_BODY()

public:
	AGW_CardBase();

	// Getters
	AGW_RowBase* GetOwnerRow() const;
	AGW_UnitRow* GetOwnerUnitRow() const;
	FName GetCardName() const;
	int32 GetCardPower() const;
	EUnitRowType GetCardRowType() const;
	ECardAbility GetCardAbility() const;
	UTexture2D* GetCardImage() const;
	int32 GetBaseCardPower() const;
	bool GetIsSelectable();
	bool IsWeatherCard();
	
	void SetCardPower(int32 NewCardPower);
	void SetIsSelectable(bool bNewIsSelectable);
	
	void HighlightCard(bool bHighlight);
	void EndCardAbility();

	// Row Functions
	void SetOwnerRow(AGW_RowBase* NewOwner, bool bShouldActivateAbility);
	void DetachFromOwnerRow();
	AGW_UnitRow* FindValidRow();

	bool bIsSnapped = false;
	bool bIsSpecial = false; // placed on the special slot on the row (only commander's horn)
	bool bIsHero = false; // can not be affected from buff/debuffs, i.e. immune card

	void InitializeCardData(FCardData NewCardData);
	void DestroySelf();
	void DestroySelfAfterDelay(float Delay = 1.f);

	// Card Power Parameters
	bool bHasWeatherDamage = false;
	bool bHasHornBoost = false;
	int32 TightBondMultiplier = 1;
	int32 MoraleBoost = 0;
	void CalculatePower();

	UPROPERTY() AGW_Graveyard* Graveyard;

	EPlayerID PlayerID; // Card Owner's PlayerID

	FOnCardAbilityEndedSignature OnCardAbilityEnded;
	
protected:
	virtual void BeginPlay() override;

	void CanActivateAbility();

	UPROPERTY() AGW_RowBase* OwnerRow;
	int32 BaseCardPower;
	bool bIsSelectable = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* CardPowerText;
	
	// Card Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	FName CardName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	int32 CardPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	EUnitRowType CardRowType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	ECardAbility CardAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	UTexture2D* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardData")
	USoundBase* CardSFX;
	
private:
	// for editor
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FTimerHandle DestroyTimer;
	
};

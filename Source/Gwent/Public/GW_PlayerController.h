// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "Data/GW_PlayerData.h"
#include "GameFramework/PlayerController.h"
#include "GW_PlayerController.generated.h"

class AGW_GameMode;
class UInputMappingContext;
class UGW_AbilityMedic;
class UW_MedicAbility;
class UInputAction;
class AGW_UnitRow;
class AGW_CardBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoldPassTurnSignature, bool bHoldStarted);

/**
 * 
 */
UCLASS()
class GWENT_API AGW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGW_PlayerController();

	void StartTurn();
	
	// UI
	void ShowMedicAbilityWidget(UGW_AbilityMedic* OwningAbility);
	
	FOnHoldPassTurnSignature OnHoldPassTurn;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
	void SetPlayerTurnInputMapping();
	
	UFUNCTION() void OnPlayerDataChanged(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID);
	
	void OnClicked();

	// Pass Turn functions
	void OnHoldPassTurnStarted();
	void OnHoldPassTurnCancelled();
	void OnPassedTurnTriggered();
	
	// line trace functions
	AGW_UnitRow* GetRowUnderCursor(AGW_CardBase* CardToCheck);
	AGW_CardBase* GetCardUnderCursor();
	
	void OnPlayedCardAbilityEnded();
	
	EPlayerID PlayerControllerID = EPlayerID::Player1; // ID is always Player1 for single-player mode
	bool bIsPlayerControllable = false;
	UPROPERTY() AGW_CardBase* SelectedCard;
	UPROPERTY() AGW_GameMode* GameMode;
	
	// UI
	UPROPERTY()
	UW_MedicAbility* MedicAbilityWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UW_MedicAbility> MedicAbilityWidgetClass;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PlayerTurnMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* NonPlayerTurnMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HoldSpaceAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ClickSFX;
	
};

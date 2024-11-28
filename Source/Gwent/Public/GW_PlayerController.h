// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "GameFramework/PlayerController.h"
#include "GW_PlayerController.generated.h"

class UInputAction;
class AGW_UnitRow;
class AGW_CardBase;
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

protected:
	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* PlayerInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HoldSpaceAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ClickSFX;
	
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
	// ID is always Player1 for single-player mode
	EPlayerID PlayerControllerID = EPlayerID::Player1;

	UPROPERTY() AGW_CardBase* SelectedCard;
	
private:
	void OnClicked();
	void OnPassedTurn();
	AGW_CardBase* GetCardUnderCursor();
	AGW_UnitRow* GetRowUnderCursor(AGW_CardBase* CardToCheck);

};

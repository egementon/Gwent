// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GW_Types.h"
#include "GameFramework/PlayerController.h"
#include "GW_PlayerController.generated.h"

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
	UPROPERTY() AGW_CardBase* SelectedCard;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ClickSFX;

	// ID is always Player1 for single-player mode
	EPlayerID PlayerControllerID = EPlayerID::Player1;

private:
	void OnClicked();
	AGW_CardBase* GetCardUnderCursor();
	AGW_UnitRow* GetRowUnderCursor(AGW_CardBase* CardToCheck);

};

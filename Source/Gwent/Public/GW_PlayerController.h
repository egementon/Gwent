// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GW_PlayerController.generated.h"

class AGW_Row;
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

	// The card currently being dragged
	UPROPERTY()
	AGW_CardBase* DraggedCard;

	// Start dragging a card
	void StartDrag();

	// Stop dragging and snap the card to the nearest row
	void StopDrag();

	// Update the card's position while dragging
	void StartUpdateDrag();
	void StopUpdateDrag();
	void UpdateDrag();

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	TArray<AGW_Row*> RowArray;

	// every frame timer for updating the card's position while dragging
	FTimerHandle UpdateDragTimerHandle;



private:
	// Raycast to detect card under the cursor
	AGW_CardBase* GetCardUnderCursor();
};

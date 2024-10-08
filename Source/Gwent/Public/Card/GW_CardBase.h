// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// Card Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardPower = 1;
	
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

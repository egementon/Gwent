// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_PlayerController.h"

#include "Camera/CameraActor.h"
#include "Gwent/Public/GW_Row.h"
#include "Gwent/Public/Card/GW_CardBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


AGW_PlayerController::AGW_PlayerController()
{
    DraggedCard = nullptr;
    bShowMouseCursor = true;
}

void AGW_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Bind input for dragging cards
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AGW_PlayerController::StartDrag);
    InputComponent->BindAction("LeftClick", IE_Released, this, &AGW_PlayerController::StopDrag);
}

void AGW_PlayerController::BeginPlay()
{
    // Set the player's view to the camera actor
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        ACameraActor* CameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass()));
        if (CameraActor)
        {
            PlayerController->SetViewTarget(CameraActor);
        }
    }
    
    // Initialize RowArray
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(this, AGW_Row::StaticClass(), Actors);
    for (AActor* Actor : Actors)
    {
        RowArray.AddUnique(Cast<AGW_Row>(Actor));
    }
}

void AGW_PlayerController::Tick(float DeltaSeconds)
{
    if (DraggedCard)
    {
        UpdateDrag();
    }
}

void AGW_PlayerController::StartDrag()
{
    // Perform a raycast to find the card under the cursor
    DraggedCard = GetCardUnderCursor();
    if (DraggedCard && DraggedCard->bIsSnapped)
    {
        DraggedCard->DetachFromOwnerRow();
    }
}

void AGW_PlayerController::StopDrag()
{
    // TODO: It Should work with Collisions not distance, because rows are rectangle not uniform
    // Snap the card to the nearest row when dropping
    if (DraggedCard)
    {
        FVector CardLocation = DraggedCard->GetActorLocation();
        
        float Distance = MAX_FLT;
        int32 ClosestRowIndex = 0;

        for (int32 i = 0; i < RowArray.Num(); i++)
        {
            float NewDistance = FVector::Dist2D(CardLocation, RowArray[i]->GetActorLocation());
            if (NewDistance < Distance && !RowArray[i]->bIsPlayerDeck)
            {
                Distance = NewDistance;
                ClosestRowIndex = i;
            }
        }
        // if (Distance < 500.f)
        // {
        //     DraggedCard->SetNewOwnerRow(RowArray[ClosestRowIndex]);
        // }
        DraggedCard->SetNewOwnerRow(RowArray[ClosestRowIndex]);
        
        DraggedCard = nullptr;
    }
}

void AGW_PlayerController::UpdateDrag()
{
    // Update card's location to follow mouse
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    DraggedCard->SetActorLocation(HitResult.Location);
    
    // if (HitResult.GetActor())
    // {
    //     UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("HitActor: %s"), *HitResult.GetActor()->GetName()));
    // }
    // DrawDebugSphere(GetWorld(), HitResult.Location, 40.f, 12, FColor::Red);
}

AGW_CardBase* AGW_PlayerController::GetCardUnderCursor()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.GetActor()->IsValidLowLevel())
    {
        // Check if the hit actor is a card
        return Cast<AGW_CardBase>(HitResult.GetActor());
    }
    
    return nullptr;
}
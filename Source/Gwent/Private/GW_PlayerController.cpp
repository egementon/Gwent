// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_PlayerController.h"

#include "GW_GameMode.h"
#include "Row/GW_PlayerHand.h"
#include "Camera/CameraActor.h"
#include "Row/GW_UnitRow.h"
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

    RowArray = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->RowArray;
}

void AGW_PlayerController::Tick(float DeltaSeconds)
{
}

void AGW_PlayerController::StartDrag()
{
    // Perform a raycast to find the card under the cursor
    DraggedCard = GetCardUnderCursor();
    if (DraggedCard && DraggedCard->bIsSnapped)
    {
        DraggedCard->DetachFromOwnerRow();
        StartUpdateDrag();
    }
}

void AGW_PlayerController::StopDrag()
{
    // TODO: It should work with Collisions not distance, because rows are rectangle not uniform like sphere
    // Snap the card to the nearest row (which is not PlayerHand) when dropping
    if (DraggedCard)
    {
        FVector CardLocation = DraggedCard->GetActorLocation();
        
        float Distance = MAX_FLT;
        int32 ClosestRowIndex = 0;
        bool bFoundValidRow = false;

        for (int32 i = 0; i < RowArray.Num(); i++)
        {
            if (RowArray[i]->IsValidRowForCard(DraggedCard))
            {
                float NewDistance = FVector::Dist2D(CardLocation, RowArray[i]->GetActorLocation());
                if (NewDistance < Distance && NewDistance < 650.f) // should be closer than 650 units to snap
                {
                    Distance = NewDistance;
                    ClosestRowIndex = i;
                    bFoundValidRow = true;
                }
            }
        }

        if (bFoundValidRow)
        {
            DraggedCard->SetOwnerRow(RowArray[ClosestRowIndex], true);
        }
        else
        {
            AGW_PlayerHand* PlayerHand = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerHand;
            DraggedCard->SetOwnerRow(PlayerHand, false);
        }
        StopUpdateDrag();
        DraggedCard = nullptr;
    }
}

void AGW_PlayerController::StartUpdateDrag()
{
    // Start calling UpdateDrag() every frame (DeltaSeconds = 0.0 means every tick)
    GetWorld()->GetTimerManager().SetTimer(UpdateDragTimerHandle, this, &AGW_PlayerController::UpdateDrag, 0.01f, true);
}

void AGW_PlayerController::StopUpdateDrag()
{
    // Stop the timer to stop calling UpdateDrag()
    GetWorld()->GetTimerManager().ClearTimer(UpdateDragTimerHandle);
}

void AGW_PlayerController::UpdateDrag()
{
    // Update card's location to follow mouse
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    DraggedCard->SetActorLocation(FVector(HitResult.Location.X, HitResult.Location.Y, DraggedCard->GetActorLocation().Z));

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
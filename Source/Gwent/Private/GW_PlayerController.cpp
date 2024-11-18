// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_PlayerController.h"

#include "GW_FuncLib.h"
#include "Camera/CameraActor.h"
#include "Row/GW_UnitRow.h"
#include "Gwent/Public/Card/GW_CardBase.h"
#include "Kismet/GameplayStatics.h"


AGW_PlayerController::AGW_PlayerController()
{
    SelectedCard = nullptr;
    bShowMouseCursor = true;
}

void AGW_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AGW_PlayerController::OnClicked);
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
}

void AGW_PlayerController::OnClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSFX);

    if (!UGW_FuncLib::CheckIsPlayerIDTurn(GetWorld(), PlayerControllerID))
    {
        return;
    }
    
    if (SelectedCard)
    {
        AGW_UnitRow* SelectedRow = GetRowUnderCursor(SelectedCard);
        
        if (SelectedRow && SelectedRow->IsValidRowForCard(SelectedCard))
        {
            SelectedCard->DetachFromOwnerRow();
            SelectedCard->SetOwnerRow(SelectedRow, true);
        }
        
        SelectedCard->HighlightCard(false);
        SelectedCard = nullptr;
    }
    else
    {
        SelectedCard = GetCardUnderCursor();
        
        if (SelectedCard && SelectedCard->bIsSnapped)
        {
            SelectedCard->HighlightCard(true);
        }
    }
}


AGW_UnitRow* AGW_PlayerController::GetRowUnderCursor(AGW_CardBase* CardToCheck)
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_GameTraceChannel2, false, HitResult); // Trace_Row channel
    if (HitResult.GetActor()->IsValidLowLevel())
    {
        if (AGW_UnitRow* Row = Cast<AGW_UnitRow>(HitResult.GetActor()))
        {
            // verify if the selected row section is a valid placement for the card, based on whether it is a special card
            const bool bIsCardSpecial = CardToCheck->bIsSpecial;
            const UPrimitiveComponent* HitComp = HitResult.GetComponent();
            if ((!bIsCardSpecial && HitComp->ComponentHasTag(FName("RowSlot"))) || (bIsCardSpecial && HitComp->ComponentHasTag(FName("SpecialSlot"))))
            {
                return Row;
            }
        }
    }
    
    return nullptr;
}

AGW_CardBase* AGW_PlayerController::GetCardUnderCursor()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult); // Trace_Card channel

    if (HitResult.GetActor()->IsValidLowLevel())
    {
        if (AGW_CardBase* CardUnderCursor = Cast<AGW_CardBase>(HitResult.GetActor()))
        {
            // can only select cards that have same ID 
            if (PlayerControllerID == CardUnderCursor->PlayerID && CardUnderCursor->GetIsSelectable())
            {
                return CardUnderCursor;
            }
        }
    }
    
    return nullptr;
}

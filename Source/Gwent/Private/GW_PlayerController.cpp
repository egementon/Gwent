// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Data/GW_PlayerData.h"
#include "Row/GW_UnitRow.h"
#include "Gwent/Public/Card/GW_CardBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/W_MedicAbility.h"


AGW_PlayerController::AGW_PlayerController()
{
    SelectedCard = nullptr;
    bShowMouseCursor = true;
}

void AGW_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind Input Actions
        EnhancedInput->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AGW_PlayerController::OnClicked);
        EnhancedInput->BindAction(HoldSpaceAction, ETriggerEvent::Started, this, &AGW_PlayerController::OnHoldPassTurnStarted);
        EnhancedInput->BindAction(HoldSpaceAction, ETriggerEvent::Canceled, this, &AGW_PlayerController::OnHoldPassTurnCancelled);
        EnhancedInput->BindAction(HoldSpaceAction, ETriggerEvent::Triggered, this, &AGW_PlayerController::OnPassedTurnTriggered);
    }
}

void AGW_PlayerController::SetPlayerTurnInputMapping()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings(); // Remove all existing mappings

        if (bIsPlayerControllable)
        {
            Subsystem->AddMappingContext(PlayerTurnMappingContext, 1); // Enable full player control
        }
        else
        {
            Subsystem->AddMappingContext(NonPlayerTurnMappingContext, 1); // Enable only limited control
        }
    }
}

void AGW_PlayerController::BeginPlay()
{
    // start the game with non-PlayerTurn input mapping
    SetPlayerTurnInputMapping(); 
    
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

    GameMode = UGW_FuncLib::GetGameMode(GetWorld());
    GameMode->Player1Data->OnPlayerDataChanged.AddDynamic(this, &AGW_PlayerController::OnPlayerDataChanged);
}

void AGW_PlayerController::OnPlayerDataChanged(UGW_PlayerData* UpdatedPlayerData, int32 PlayerID)
{
    if (PlayerID == 1)
    {
        bIsPlayerControllable = UpdatedPlayerData->Data.IsPlayerTurn && !UpdatedPlayerData->Data.PassedTurn;
        SetPlayerTurnInputMapping();
    }
}

void AGW_PlayerController::StartTurn()
{
    if (GameMode->Player1Data->IsTurnPassed())
    {
        OnPassedTurnTriggered();
    }
}

void AGW_PlayerController::OnClicked()
{
    UGameplayStatics::PlaySound2D(this, ClickSFX);

    if (!bIsPlayerControllable)
    {
        return;
    }
    
    if (SelectedCard)
    {
        AGW_UnitRow* SelectedRow = GetRowUnderCursor(SelectedCard);
        
        if (SelectedRow && SelectedRow->IsValidRowForCard(SelectedCard))
        {
            SelectedCard->OnCardAbilityEnded.AddLambda([this]()
            {
                if (GameMode->Player1Data->GetHandSize() == 0)
                {
                    GameMode->PlayerPassedTurn(PlayerControllerID);
                }
                else
                {
                    GameMode->EndPlayerTurn(PlayerControllerID);
                }
            });
            
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

void AGW_PlayerController::OnHoldPassTurnStarted()
{
    OnHoldPassTurn.Broadcast(true);
}

void AGW_PlayerController::OnHoldPassTurnCancelled()
{
    OnHoldPassTurn.Broadcast(false);
}

void AGW_PlayerController::OnPassedTurnTriggered()
{
    GameMode->PlayerPassedTurn(PlayerControllerID);
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

void AGW_PlayerController::ShowMedicAbilityWidget(UGW_AbilityMedic* OwningAbility)
{
    MedicAbilityWidget = CreateWidget<UW_MedicAbility>(this, MedicAbilityWidgetClass);
    MedicAbilityWidget->SetMedicAbility(OwningAbility);
    if (MedicAbilityWidget)
    {
        MedicAbilityWidget->AddToViewport();
    }
}

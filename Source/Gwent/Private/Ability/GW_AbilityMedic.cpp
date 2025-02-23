// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityMedic.h"

#include "GW_PlayerController.h"
#include "Card/GW_CardBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Row/GW_Graveyard.h"
#include "Row/GW_UnitRow.h"

void UGW_AbilityMedic::OnCardSelected(AGW_CardBase* SelectedCard)
{
	if (SelectedCard)
	{
		SelectedCard->DetachAndSetOwnerRow(SelectedCard->FindValidRow(), true); // find first valid row
		
		// do not EndAbility if selected card is another medic card
		if (SelectedCard->GetCardAbility() != ECardAbility::Medic)
		{
			EndAbility();
		}
	}
}

void UGW_AbilityMedic::ActivateAbility(AGW_CardBase* Card)
{
	Super::ActivateAbility(Card);
	
	if (AGW_Graveyard* Graveyard = Card->Graveyard)
	{
		TArray<AGW_CardBase*> GraveyardCards = Graveyard->GetSnappedCardsArray();
		if (GraveyardCards.IsEmpty())
		{
			EndAbility();
			return;
		}
		
		for (AGW_CardBase* GraveyardCard : GraveyardCards)
		{
            // look for unit cards (exclude special, hero cards)
            if (GraveyardCard->IsRegularUnitCard())
			{
				SelectableGraveyardCards.AddUnique(GraveyardCard);
			}
		}

		if (!SelectableGraveyardCards.IsEmpty())
		{
			// ability behaves different for Player1 and AI
			// for Player1, show card selection
			if (Card->PlayerID == EPlayerID::Player1)
			{
				// found a unit card, display UI
				UKismetSystemLibrary::PrintString(this, "found a unit card, display UI");
				
				if (AGW_PlayerController* PC = Cast<AGW_PlayerController>(Card->GetWorld()->GetFirstPlayerController()))
				{
					PC->ShowMedicAbilityWidget(this);
				}
			}
			else // for AI, after a delay, select a card automatically
			{
				Card->GetWorld()->GetTimerManager().SetTimer(DelayTimer,[this]()
				{
					const int32 RandomIndex = FMath::RandRange(0, SelectableGraveyardCards.Num() - 1);
					OnCardSelected(SelectableGraveyardCards[RandomIndex]);
				},
			1.f,
			false);}
		}
		else
		{
			EndAbility();
		}
	}
}

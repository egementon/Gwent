// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityClearWeather.h"

#include "GW_GameMode.h"
#include "GW_Row.h"
#include "Card/GW_CardBase.h"
#include "GameFramework/GameModeBase.h"

void UGW_AbilityClearWeather::ActivateAbility(AGW_CardBase* Card)
{
	// remove Bad Weather cards from all rows and destroy them
	TArray<AGW_Row*> RowArray = Cast<AGW_GameMode>(Card->GetWorld()->GetAuthGameMode())->RowArray;

	for (int i = 0; i < RowArray.Num(); ++i)
	{
		if (!RowArray[i]->IsSpecialSlotEmpty() && RowArray[i]->GetSnappedSpecialCard()->IsValidLowLevel())
		{
			if (RowArray[i]->GetSnappedSpecialCard()->GetCardAbility() == ECardAbility::BadWeather)
			{
				RowArray[i]->GetSnappedSpecialCard()->DestroySelfAfterDelay(0.15f);

				// remove weather debuffs from all affected cards
				TArray<AGW_CardBase*> RowCards = RowArray[i]->GetSnappedCardsArray();
				for (AGW_CardBase* RowCard : RowCards)
				{
					RowCard->SetCardPower(RowCard->GetBaseCardPower());
				}

				RowArray[i]->CalculateRowPower();
			}
		}
	}
	
	// destroy Clear Weather card
	Card->DestroySelfAfterDelay();

}

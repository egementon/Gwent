// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityMuster.h"

#include "GW_GameMode.h"
#include "GW_Row.h"
#include "Card/GW_CardBase.h"

void UGW_AbilityMuster::ActivateAbility(AGW_CardBase* Card)
{
	// TODO: expand it with not only hand but also deck
	AGW_Row* PlayerDeck = Cast<AGW_GameMode>(Card->GetWorld()->GetAuthGameMode())->PlayerDeck;
	TArray<AGW_CardBase*> PlayerDeckCards = PlayerDeck->GetSnappedCardsArray();
	TArray<AGW_CardBase*> SameNameDeckCards;
	
	for (AGW_CardBase* DeckCard : PlayerDeckCards)
	{
		if (Card->CardName == DeckCard->CardName)
		{
			SameNameDeckCards.Add(DeckCard);
		}
	}

	if (SameNameDeckCards.Num() > 0)
	{
		for (AGW_CardBase* SameNameRowCard : SameNameDeckCards)
		{
	        SameNameRowCard->DetachFromOwnerRow();
			SameNameRowCard->SetOwnerRow(Card->GetOwnerRow(), false);
		}
	}

	Super::ActivateAbility(Card);
}

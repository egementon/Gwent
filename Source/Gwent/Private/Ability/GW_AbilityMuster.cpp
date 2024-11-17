// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilityMuster.h"

#include "GW_GameMode.h"
#include "Row/GW_PlayerHand.h"
#include "Card/GW_CardBase.h"
#include "Row/GW_Deck.h"

void UGW_AbilityMuster::ActivateAbility(AGW_CardBase* Card)
{
	const AGW_GameMode* GameMode = Cast<AGW_GameMode>(Card->GetWorld()->GetAuthGameMode());

	// get all of the player cards
	AGW_PlayerHand* PlayerHand;
	AGW_Deck* Deck;
	if (Card->PlayerID == EPlayerID::Player1)
	{
		PlayerHand = GameMode->PlayerHandP1;
		Deck = GameMode->DeckP1;
	}
	else
	{
		PlayerHand = GameMode->PlayerHandP2;
		Deck = GameMode->DeckP2;
	}
	
	TArray<AGW_CardBase*> AllPlayerCards = PlayerHand->GetSnappedCardsArray();
	AllPlayerCards.Append(Deck->GetSnappedCardsArray());

	TArray<AGW_CardBase*> SameNameDeckCards;

	// check for same name cards
	for (AGW_CardBase* DeckCard : AllPlayerCards)
	{
		if (Card->GetCardName() == DeckCard->GetCardName())
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
}

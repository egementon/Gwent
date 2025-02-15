// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GW_AbilitySpy.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"
#include "Row/GW_Deck.h"
#include "Row/GW_PlayerHand.h"

void UGW_AbilitySpy::ActivateAbility(AGW_CardBase* Card)
{
	const AGW_GameMode* GameMode = UGW_FuncLib::GetGameMode(Card->GetWorld());
	
	if (Card->PlayerID != EPlayerID::Player1) // reason for != is that spy cards change ID after played on opponent row 
	{
		PlayerDeck = GameMode->DeckP1;
		PlayerHand = GameMode->PlayerHandP1;
	}
	else
	{
		PlayerDeck = GameMode->DeckP2;
		PlayerHand = GameMode->PlayerHandP2;
	}

	if (PlayerDeck->GetSnappedCardsArray().IsEmpty()) return;

	// get 2 cards from deck to hand
	for (int i = 0; i < 2; ++i)
	{
		GetRandomCardFromDeckToHand();
	}
}

void UGW_AbilitySpy::GetRandomCardFromDeckToHand()
{
	if (PlayerDeck && PlayerHand)
	{
		TArray<AGW_CardBase*> PlayerDeckCards = PlayerDeck->GetSnappedCardsArray();
		if (PlayerDeckCards.IsEmpty()) return;
		
		const int32 RandomIndex = FMath::RandRange(0, PlayerDeckCards.Num() - 1);

		PlayerDeckCards[RandomIndex]->DetachFromOwnerRow();
		PlayerDeckCards[RandomIndex]->SetOwnerRow(PlayerHand, false);
		PlayerDeckCards[RandomIndex]->SetIsSelectable(true);
	}
}

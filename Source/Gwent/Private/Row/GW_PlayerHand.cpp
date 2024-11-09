// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_PlayerHand.h"

#include "GW_GameMode.h"
#include "Card/GW_CardBase.h"


AGW_PlayerHand::AGW_PlayerHand()
{
}


void AGW_PlayerHand::GenerateRandomCards()
{
	if (!CardDataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No card data available."));
		return;
	}
	
	TArray<FCardData> SelectableCards = CardDataAsset->CardDataArray;

	// remove excluded cards from SelectableCards
	for (int32 i = SelectableCards.Num() - 1; i >= 0; --i)
	{
		if (SelectableCards[i].bDebug_ExcludeFromDeck == true)
		{
			SelectableCards.RemoveAt(i);
		}
	}

	if (SelectableCards.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No suitable card."));
		return;
	}

	AGW_Graveyard* Graveyard = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->Graveyard;
	
	for (int i = 0; i < 10; ++i)
	{
		// Randomly select card data from the data asset array
		const int32 RandomIndex = FMath::RandRange(0, SelectableCards.Num() - 1);
		const FCardData& SelectedCardData = SelectableCards[RandomIndex];

		// Spawn the card
		auto Card = GetWorld()->SpawnActorDeferred<AGW_CardBase>(CardClass, FTransform::Identity);
		Card->InitializeCardData(SelectedCardData);
		Card->SetOwnerRow(this, false);
		Card->Graveyard = Graveyard;
		Card->FinishSpawning(FTransform::Identity);
	}

	UpdateCardsLocations();
}


void AGW_PlayerHand::BeginPlay()
{
	Super::BeginPlay();

	// add self reference to GameMode
	Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerHand = this;
}



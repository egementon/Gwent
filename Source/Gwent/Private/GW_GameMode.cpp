// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/GW_GameMode.h"

#include "GW_Row.h"
#include "Kismet/GameplayStatics.h"

void AGW_GameMode::BeginPlay()
{
	// Initialize RowArray and PlayerDeck
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AGW_Row::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (AGW_Row* Row = Cast<AGW_Row>(Actor))
		{
			if (!Row->bIsPlayerDeck)
			{
				RowArray.AddUnique(Row);
			}
			else
			{
				PlayerDeck = Row;
			}
		}
	}
}

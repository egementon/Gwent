// Fill out your copyright notice in the Description page of Project Settings.


#include "Row/GW_Graveyard.h"


AGW_Graveyard::AGW_Graveyard()
{
	PrimaryActorTick.bCanEverTick = false;

	CardSpacingX = 3.f;
	bVerticalOffset = true;
}

void AGW_Graveyard::BeginPlay()
{
	Super::BeginPlay();
}

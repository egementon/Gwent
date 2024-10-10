﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/Card/GW_CardBase.h"

#include "GW_Row.h"
#include "Components/TextRenderComponent.h"


// Sets default values
AGW_CardBase::AGW_CardBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	CardPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	CardPowerText->SetupAttachment(CardMesh);

	CardPowerText->SetText(FText::AsNumber(CardPower));
}

void AGW_CardBase::InitializeCardData(FCardData NewCardData)
{
	CardName = NewCardData.Name;
	CardPower = NewCardData.Power;
	CardRowType = NewCardData.RowType;
	CardAbility = NewCardData.Ability;
	CardIcon = NewCardData.Icon;
	ColorMaterial = NewCardData.ColorMaterial;
}

void AGW_CardBase::BeginPlay()
{
	Super::BeginPlay();

	if (ColorMaterial)
	{
		CardMesh->SetMaterial(0, ColorMaterial);
	}
	CardPowerText->SetText(FText::AsNumber(CardPower));
}

void AGW_CardBase::SetNewOwnerRow(AGW_Row* NewOwner)
{
	NewOwner->AddToCardsArray(this);
	OwnerRow = NewOwner;
	bIsSnapped = true;
}

void AGW_CardBase::DetachFromOwnerRow()
{
	OwnerRow->RemoveFromCardsArray(this);
	OwnerRow = nullptr;
	bIsSnapped = false;
}

void AGW_CardBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGW_CardBase, CardPower))
	{
		CardPowerText->SetText(FText::AsNumber(CardPower));
	}
}


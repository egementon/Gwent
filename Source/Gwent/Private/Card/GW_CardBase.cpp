// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/Card/GW_CardBase.h"

#include "GW_GameMode.h"
#include "GW_Row.h"
#include "Ability/Core/GW_AbilityBase.h"
#include "Ability/Core/GW_AbilityManager.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AGW_CardBase::AGW_CardBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	CardPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	CardPowerText->SetupAttachment(CardMesh);
	
	SetCardPower(CardPower);
}

AGW_Row* AGW_CardBase::GetOwnerRow()
{
	return OwnerRow;
}

int32 AGW_CardBase::GetCardPower()
{
	return CardPower;
}

void AGW_CardBase::SetCardPower(int32 NewCardPower)
{
	CardPower = NewCardPower;
	CardPowerText->SetText(FText::AsNumber(CardPower));

	const FColor PowerColor = (CardPower != BaseCardPower) 
	? (CardPower > BaseCardPower ? FColor::Green : FColor::Red) 
	: FColor::Black;
	
	CardPowerText->SetTextRenderColor(PowerColor);
}

int32 AGW_CardBase::GetBaseCardPower()
{
	return BaseCardPower;
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
	
	BaseCardPower = CardPower;
	SetCardPower(CardPower);
}

void AGW_CardBase::CanActivateAbility()
{
	if (CardAbility == ECardAbility::NoAbility) return;

	UGW_AbilityBase* Ability = UGW_AbilityManager::Get()->GetAbility(CardAbility);
	if (Ability)
	{
		Ability->ActivateAbility(this);
	}
}

void AGW_CardBase::SetOwnerRow(AGW_Row* NewOwner, bool bShouldActivateAbility)
{
	NewOwner->AddToCardsArray(this);
	OwnerRow = NewOwner;
	bIsSnapped = true;

	// try to activate card ability if placed on a battle row
	if (bShouldActivateAbility)
	{
		CanActivateAbility();
	}
}

void AGW_CardBase::DetachFromOwnerRow()
{
	OwnerRow->RemoveFromCardsArray(this);
	OwnerRow = nullptr;
	bIsSnapped = false;
}

void AGW_CardBase::SetOwnerRowAsPlayerDeck()
{
	AGW_Row* PlayerDeck = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerDeck;
	SetOwnerRow(PlayerDeck, false);
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


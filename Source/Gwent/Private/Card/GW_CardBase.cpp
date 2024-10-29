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
	SetRootComponent(CardMesh);
	
	CardPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	CardPowerText->SetupAttachment(CardMesh);

	CardPowerText->SetText(FText::AsNumber(CardPower));
	CardPowerText->SetTextRenderColor(FColor::Black);
}

AGW_Row* AGW_CardBase::GetOwnerRow() const
{
	return OwnerRow;
}

FName AGW_CardBase::GetCardName() const
{
	return CardName;
}

int32 AGW_CardBase::GetCardPower() const
{
	return CardPower;
}

ECardRowType AGW_CardBase::GetCardRowType() const
{
	return CardRowType;
}

ECardAbility AGW_CardBase::GetCardAbility() const
{
	return CardAbility;
}

void AGW_CardBase::SetCardPower(int32 NewCardPower)
{
	if (bIsHero) return; // hero cards are immune to buff/debuffs
	
	CardPower = NewCardPower;
	CardPowerText->SetText(FText::AsNumber(CardPower));

	constexpr FColor DarkGreen = FColor(0, 70, 0);
	constexpr FColor DarkRed = FColor(70, 0, 0);

	const FColor PowerColor = (CardPower != BaseCardPower) 
	? (CardPower > BaseCardPower ? DarkGreen : DarkRed) 
	: FColor::Black;
	
	CardPowerText->SetTextRenderColor(PowerColor);
}

int32 AGW_CardBase::GetBaseCardPower() const
{
	return BaseCardPower;
}

void AGW_CardBase::InitializeCardData(FCardData NewCardData)
{
	CardName = NewCardData.Name;
	CardPower = NewCardData.Power;
	CardRowType = NewCardData.RowType;
	bIsSpecial = NewCardData.bIsSpecial;
	bIsHero = NewCardData.bIsHero;
	CardAbility = NewCardData.Ability;
	CardImage = NewCardData.Image;
	BaseCardPower = CardPower;
}

void AGW_CardBase::DestroySelf()
{
	// card specific destroy events
	if (CardAbility == ECardAbility::MoraleBoost)
	{
		GetOwnerRow()->RowMoraleBoostAddition--;
		GetOwnerRow()->UpdateAllCardsPowers();
	}
	if (CardAbility == ECardAbility::TightBond)
	{
		GetOwnerRow()->OnTightBondedCardRemoved(this);
		GetOwnerRow()->UpdateAllCardsPowers();
	}
	if (CardAbility == ECardAbility::BadWeather)
	{
		GetOwnerRow()->bRowHasBadWeather = false;
		GetOwnerRow()->UpdateAllCardsPowers();
	}

	// general destroy events
	if (bIsSpecial)
	{
		OwnerRow->SetSpecialCard(nullptr);
		OwnerRow->SetSpecialSlotEmpty(true);
	}
	else
	{
		OwnerRow->RemoveFromCardsArray(this);
	}
	
	Destroy();
}

void AGW_CardBase::DestroySelfAfterDelay(const float Delay)
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AGW_CardBase::DestroySelf, Delay, false);
}

void AGW_CardBase::CalculatePower()
{
	const int32 Power = ((bHasWeatherDamage ? 1 : BaseCardPower) * TightBondMultiplier + MoraleBoost) * (bHasHornBoost ? 2 : 1);
	SetCardPower(Power);
}

void AGW_CardBase::BeginPlay()
{
	Super::BeginPlay();

	if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CardMesh->GetMaterial(0), this))
	{
		DynamicMaterial->SetTextureParameterValue(FName("CardTexture"), CardImage);
		CardMesh->SetMaterial(0, DynamicMaterial);
	}

	// Card Power is already written in Hero Cards. Also some cards do not have any power to be written (0)
	if (CardPower == 0 || bIsHero)
	{
		CardPowerText->SetVisibility(false);
	}
	else
	{
		CardPowerText->SetText(FText::AsNumber(CardPower));
		CardPowerText->SetTextRenderColor(FColor::Black);
	}
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

void AGW_CardBase::SetOwnerRow(AGW_Row* NewOwner, const bool bShouldActivateAbility)
{
	OwnerRow = NewOwner;
	bIsSnapped = true;

	// try to activate card ability
	if (bShouldActivateAbility)
	{
		CanActivateAbility();
	}
	
	if (bIsSpecial)
	{
		NewOwner->SetSpecialCard(this);
	}
	else
	{
		NewOwner->AddToCardsArray(this);
	}
}

void AGW_CardBase::DetachFromOwnerRow()
{
	OwnerRow->RemoveFromCardsArray(this);
	OwnerRow = nullptr;
	bIsSnapped = false;
}

void AGW_CardBase::SetOwnerRowAsPlayerHand()
{
	AGW_Row* PlayerHand = Cast<AGW_GameMode>(GetWorld()->GetAuthGameMode())->PlayerHand;
	PlayerHand->AddToCardsArray(this);
	OwnerRow = PlayerHand;
	bIsSnapped = true;
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


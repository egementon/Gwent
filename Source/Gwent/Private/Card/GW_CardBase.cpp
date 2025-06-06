﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Gwent/Public/Card/GW_CardBase.h"

#include "GW_FuncLib.h"
#include "GW_GameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Ability/Core/GW_AbilityBase.h"
#include "Ability/Core/GW_AbilityManager.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Row/GW_Deck.h"
#include "Row/GW_Graveyard.h"
#include "Row/GW_PlayerHand.h"
#include "Row/GW_UnitRow.h"
#include "Row/GW_WeatherRow.h"


// Sets default values
AGW_CardBase::AGW_CardBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(CardMesh);
	CardMesh->SetCollisionProfileName("Card");
	
	CardPowerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	CardPowerText->SetupAttachment(CardMesh);

	CardPowerText->SetText(FText::AsNumber(CardPower));
	CardPowerText->SetTextRenderColor(FColor::Black);
}

AGW_RowBase* AGW_CardBase::GetOwnerRow() const
{
	return OwnerRow;
}

AGW_UnitRow* AGW_CardBase::GetOwnerUnitRow() const
{
	return Cast<AGW_UnitRow>(OwnerRow);
}

FName AGW_CardBase::GetCardName() const
{
	return CardName;
}

int32 AGW_CardBase::GetCardPower() const
{
	return CardPower;
}

EUnitRowType AGW_CardBase::GetCardRowType() const
{
	return CardRowType;
}

ECardAbility AGW_CardBase::GetCardAbility() const
{
	return CardAbility;
}

UTexture2D* AGW_CardBase::GetCardImage() const
{
	return CardImage;
}

int32 AGW_CardBase::GetBaseCardPower() const
{
	return BaseCardPower;
}

bool AGW_CardBase::GetIsSelectable()
{
	return bIsSelectable;
}

bool AGW_CardBase::IsWeatherCard()
{
	return GetCardAbility() == ECardAbility::BadWeather || GetCardAbility() == ECardAbility::ClearWeather;
}

bool AGW_CardBase::IsRegularUnitCard()
{
	return CardType == ECardType::Unit_Regular;
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

void AGW_CardBase::ResetCardPower()
{
	SetCardPower(BaseCardPower);
}

void AGW_CardBase::SetIsSelectable(bool bNewIsSelectable)
{
	bIsSelectable = bNewIsSelectable;
}

void AGW_CardBase::SetIsFacedDown(bool bNewIsFacedDown)
{
	if (bIsFacedDown == bNewIsFacedDown) return;

	bIsFacedDown = bNewIsFacedDown;

	if (CardPower != 0 && !bIsHero)
	{
		CardPowerText->SetVisibility(!bIsFacedDown);
	}

	if (bIsFacedDown)
	{
		CardMesh->SetMaterial(0, FacedDownMaterial);
	}
	else
	{
		SetStartingMaterial();
	}
}

void AGW_CardBase::HighlightCard(bool bHighlight)
{
	CardMesh->SetRenderCustomDepth(bHighlight);
	HighlightValidRows(bHighlight);
}

void AGW_CardBase::EndCardAbility()
{
	OnCardAbilityEnded.Broadcast();
}

void AGW_CardBase::InitializeCardData(FCardData NewCardData)
{
	CardName = NewCardData.Name;
	CardPower = NewCardData.Power;
	CardType = NewCardData.CardType;
	CardRowType = NewCardData.RowType;
	CardAbility = NewCardData.Ability;
	CardImage = NewCardData.Image;
	CardSFX = NewCardData.SFX;

	BaseCardPower = CardPower;
	bIsHero = CardType == ECardType::Unit_Hero;
	bIsSpecial = CardType == ECardType::Special_SpecialSlot;
}

void AGW_CardBase::DestroySelf()
{
	DetachAndSetOwnerRow(Graveyard, false);
	SetIsSelectable(false);
	ResetCardPower();
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

	if (!bIsFacedDown)
	{
		SetStartingMaterial();
	}

	// Card Power is already written in Hero Cards. Also, some cards do not have any power to be written (0)
	if (CardPower == 0 || bIsHero || CardAbility == ECardAbility::Decoy) //TODO: create a new boolean for Powerless cards instead of giving them 0 power
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
	if (CardAbility == ECardAbility::NoAbility)
	{
		EndCardAbility();
		return;
	}

	UGW_AbilityBase* Ability = UGW_AbilityManager::Get()->GetAbility(CardAbility);
	if (Ability)
	{
		Ability->ActivateAbility(this);
	}

	// Only Medic cards does not end automatically, they wait for UI selection
	if (CardAbility != ECardAbility::Medic)
	{
		EndCardAbility();
	}
}

void AGW_CardBase::SetStartingMaterial()
{
	if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CardMesh->GetMaterial(0), this))
	{
		DynamicMaterial->SetTextureParameterValue(FName("CardTexture"), CardImage);
		CardMesh->SetMaterial(0, DynamicMaterial);
	}
}

void AGW_CardBase::DetachAndSetOwnerRow(AGW_RowBase* NewOwner, bool bShouldActivateAbility)
{
	DetachFromOwnerRow();
	SetOwnerRow(NewOwner, bShouldActivateAbility);
}

void AGW_CardBase::SetOwnerRow(AGW_RowBase* NewOwner, const bool bShouldActivateAbility)
{
	OwnerRow = NewOwner;
	PlayerID = NewOwner->GetPlayerID();
	bIsSnapped = true;

	if (Cast<AGW_UnitRow>(NewOwner)) // if NewOwner is UnitRow
	{
		// try to activate card ability
		if (bShouldActivateAbility)
		{
			CanActivateAbility();
		}
		
		// play card SFX
		if (CardSFX)
		{
			UGameplayStatics::PlaySound2D(this, CardSFX);
		}
		
		if (bIsSpecial)
		{
			Cast<AGW_UnitRow>(NewOwner)->SetSpecialCard(this);
		}
		else
		{
			NewOwner->AddToCardsArray(this);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, CardPlacedNiagaraEffect, GetActorLocation(), GetActorRotation(), FVector::One() * 7.f);

		SetIsSelectable(false);
		SetIsFacedDown(false);
	}
	else // if NewOwner is Deck, Graveyard or PlayerHand
	{
		if (Cast<AGW_Deck>(NewOwner))
		{
			SetIsFacedDown(true);
		}
		else if (Cast<AGW_Graveyard>(NewOwner))
		{
			SetIsFacedDown(false);
		}
		else if (Cast<AGW_PlayerHand>(NewOwner))
		{
			// always hide player 2 cards on PlayerHand
			if (PlayerID == EPlayerID::Player2)
			{
				SetIsFacedDown(true);
			}
			else
			{
				SetIsFacedDown(false);
			}
		}
		
		NewOwner->AddToCardsArray(this);
	}
}

void AGW_CardBase::DetachFromOwnerRow()
{
	// if card is on a UnitRow
	if (AGW_UnitRow* Row = Cast<AGW_UnitRow>(GetOwnerRow()))
	{
		// card specific destroy events
		if (CardAbility == ECardAbility::MoraleBoost)
		{
			Row->RowMoraleBoostAddition--;
			Row->UpdateAllCardsPowers();
		}
		if (CardAbility == ECardAbility::TightBond)
		{
			Row->OnTightBondedCardRemoved(this);
			Row->UpdateAllCardsPowers();
		}
		if (CardAbility == ECardAbility::BadWeather)
		{
			if (AGW_WeatherRow* WeatherRow = Cast<AGW_WeatherRow>(Row))
			{
				// Clear weather if there is no same name card on weather row left
				if (!WeatherRow->DoesRowHasSameNameCard(this))
				{
					WeatherRow->ClearWeather();
				}
			}
		}
		
		// if special card destroyed from UnitRow
		if (bIsSpecial)
		{
			Row->SetSpecialCard(nullptr);
			Row->SetSpecialSlotEmpty(true);
		}
	}
	
	OwnerRow->RemoveFromCardsArray(this);
	OwnerRow = nullptr;
	bIsSnapped = false;
}

AGW_UnitRow* AGW_CardBase::FindValidRow()
{
	AGW_UnitRow* ValidRow = nullptr;
	TArray<AGW_UnitRow*> AllRows = UGW_FuncLib::GetGameMode(GetWorld())->AllRowsArray;
	
	if (this->IsWeatherCard())
	{
		ValidRow = Cast<AGW_UnitRow>(UGW_FuncLib::GetGameMode(GetWorld())->WeatherRow);
	}
	else
	{
		// find the first valid row
		for (AGW_UnitRow* Row : AllRows)
		{
			if (Row->IsValidRowForCard(this))
			{
				ValidRow = Row;
				break;
			}
		}
	}
	
	return ValidRow;
}

void AGW_CardBase::HighlightValidRows(bool bHighlight)
{
	if (bHighlight)
	{
		if (this->IsWeatherCard())
		{
			AGW_WeatherRow* WeatherRow = UGW_FuncLib::GetGameMode(GetWorld())->WeatherRow;
			WeatherRow->HighlightRow(true);
			HighlightedRows.AddUnique(WeatherRow);
		}
		else
		{
			TArray<AGW_UnitRow*> AllRows = UGW_FuncLib::GetGameMode(GetWorld())->AllRowsArray;
			for (AGW_UnitRow* Row : AllRows)
			{
				if (Row->IsValidRowForCard(this))
				{
					if (this->bIsSpecial)
					{
						Row->HighlightRowSpecialSlot(true);
					}
					else
					{
						Row->HighlightRow(true);
					}
					
					HighlightedRows.AddUnique(Row);
				}
			}
		}
	}
	else
	{
		for (AGW_UnitRow* Row : HighlightedRows)
		{
			Row->HighlightRow(false);
			Row->HighlightRowSpecialSlot(false);
		}
		
		HighlightedRows.Empty();
	}
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


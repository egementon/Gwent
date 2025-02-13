// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_MedicAbility.h"

#include "Ability/GW_AbilityMedic.h"
#include "Blueprint/WidgetTree.h"
#include "Card/GW_CardBase.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"

void UW_MedicAbility::SetMedicAbility(UGW_AbilityMedic* NewMedicAbility)
{
	MedicAbility = NewMedicAbility;
	GraveyardCards = MedicAbility->SelectableGraveyardCards;
}

void UW_MedicAbility::NativeConstruct()
{
	Super::NativeConstruct();

	CreateCardWidgets();
}

void UW_MedicAbility::CreateCardWidgets()
{
	if (!CardScrollBox || !CardContainer) return;

	for (AGW_CardBase* Card : GraveyardCards)
	{
		if (!Card) continue;

		// Create a new button
		UButton* CardButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
		if (!CardButton) continue;

		// Create an image widget
		UImage* CardImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (!CardImage) continue;

		// Assign the texture
		if (UTexture2D* CardTexture = Card->GetCardImage())
		{
			CardImage->SetBrushFromTexture(CardTexture);
		}

		// Force the image to a fixed size
		FSlateBrush* ImageBrush = const_cast<FSlateBrush*>(&CardImage->GetBrush());
		if (ImageBrush)
		{
			ImageBrush->ImageSize = FVector2D(290, 350);
		}

		// Add image to button
		CardButton->AddChild(CardImage);

		// Store mapping
		CardButtonMap.Add(CardButton, Card);

		// Bind click event
		CardButton->OnPressed.AddDynamic(this, &UW_MedicAbility::OnCardButtonPressed);

		// Wrap Button inside a Horizontal Box Slot for proper spacing
		UHorizontalBoxSlot* CardSlot = Cast<UHorizontalBoxSlot>(CardContainer->AddChild(CardButton));
		if (CardSlot)
		{
			CardSlot->SetPadding(FMargin(10, 0, 10, 0)); // Add equal spacing between cards
			CardSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		}
	}
}

void UW_MedicAbility::OnCardButtonPressed()
{
	// Find which button was clicked
	for (const TPair<UButton*, AGW_CardBase*>& Entry : CardButtonMap)
	{
		if (Entry.Key->IsPressed())
		{
			OnSelectedCard(Entry.Value);
			RemoveFromParent();

			return;
		}
	}
}

void UW_MedicAbility::OnSelectedCard(AGW_CardBase* SelectedCard)
{
	MedicAbility->OnCardSelected(SelectedCard);
}

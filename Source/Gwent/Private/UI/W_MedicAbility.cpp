// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_MedicAbility.h"

#include "Ability/GW_AbilityMedic.h"
#include "Blueprint/WidgetTree.h"
#include "Card/GW_CardBase.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"


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

// I know this widget code is not great... but it does the job
void UW_MedicAbility::CreateCardWidgets()
{
	if (!CardScrollBox || !CardContainer) return;

	for (AGW_CardBase* Card : GraveyardCards)
	{
		if (!Card) continue;

		// Create a new button
		UButton* CardButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
		if (!CardButton) continue;

		// Create a horizontal box to hold both the image and text
		UOverlay* CardOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		if (!CardOverlay) continue;

		// Create an image widget for the card
		UImage* CardImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (!CardImage) continue;

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

		// Create a text block for the card's power
		UTextBlock* PowerText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		if (!PowerText) continue;

		// Set the text to the card's power
		PowerText->SetText(FText::AsNumber(Card->GetBaseCardPower()));
		
		// Set font
		FSlateFontInfo FontInfo;
		FontInfo.FontObject = Cast<UObject>(CardPowerFont);  // Explicitly cast UFont* to UObject*
		FontInfo.Size = 32;
		PowerText->SetFont(FontInfo);
		PowerText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
		
		// Add image to overlay
		CardOverlay->AddChildToOverlay(CardImage);

		// Add power text to overlay
		UOverlaySlot* PowerSlot = Cast<UOverlaySlot>(CardOverlay->AddChildToOverlay(PowerText));
		if (PowerSlot)
		{
			PowerSlot->SetHorizontalAlignment(HAlign_Left);
			PowerSlot->SetVerticalAlignment(VAlign_Top);
			PowerSlot->SetPadding(FMargin(29, 18, 0, 0)); // Offset from top-left
		}

		// Add overlay to the button
		CardButton->AddChild(CardOverlay);

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

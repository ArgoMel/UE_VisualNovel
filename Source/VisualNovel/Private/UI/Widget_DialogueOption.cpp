#include "UI/Widget_DialogueOption.h"
#include "UI/Widget_Dialogue.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include <Components/HorizontalBoxSlot.h>

UWidget_DialogueOption::UWidget_DialogueOption(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mOptionIndex = 0;
}

void UWidget_DialogueOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DialogueBtn->OnClicked.AddDynamic(this, &ThisClass::OnDialogueBtnClicked);
}

void UWidget_DialogueOption::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_DialogueOption::OnDialogueBtnClicked()
{
	mDialogueWidget->ChooseOption(mOptionIndex);
}

void UWidget_DialogueOption::Init(UWidget_Dialogue* DialogueWidget, FText Text, 
	int32 OptionIndex)
{
	mDialogueWidget = DialogueWidget;
	DialogueText->SetText(Text);
	mOptionIndex = OptionIndex;
}

void UWidget_DialogueOption::SetPreviouslyPickedChoice(bool UseCB, bool WasChoice)
{
	DialogueCB->SetIsChecked(WasChoice);
	UHorizontalBoxSlot* slotText = Cast<UHorizontalBoxSlot>(DialogueText->Slot);
	UHorizontalBoxSlot* slotCB = Cast<UHorizontalBoxSlot>(DialogueCB->Slot);
	if(WasChoice&&
		UseCB)
	{
		DialogueCB->SetVisibility(ESlateVisibility::HitTestInvisible);
		if (IsValid(slotText) &&
			IsValid(slotCB))
		{
			slotText->SetPadding(FMargin(30.f,0.f,0.f,0.f));
			slotCB->SetPadding(FMargin(0.f,0.f,10.f,0.f));
		}
	}
	else
	{
		DialogueCB->SetVisibility(ESlateVisibility::Collapsed);
		if (IsValid(slotText)&&
			IsValid(slotCB))
		{
			slotText->SetPadding(FMargin(0.f));
			slotCB->SetPadding(FMargin(0.f));
		}
	}
}

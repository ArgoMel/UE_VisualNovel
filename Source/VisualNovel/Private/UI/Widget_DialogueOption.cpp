#include "UI/Widget_DialogueOption.h"
#include "UI/Widget_Dialogue.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"

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
	DialogueText->SetText(mText);
}

void UWidget_DialogueOption::OnDialogueBtnClicked()
{
	mDialogueWidget->ChooseOption(mOptionIndex);
}

void UWidget_DialogueOption::Init(UWidget_Dialogue* DialogueWidget, FText Text, int32 OptionIndex)
{
	mDialogueWidget = DialogueWidget;
	mText = Text;
	mOptionIndex = OptionIndex;
}

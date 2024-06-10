#include "UI/Widget_Dialogue.h"
#include "UI/Widget_DialogueOption.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

UWidget_Dialogue::UWidget_Dialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mShowUnselectableOption = true;

	GetClassAsset(mDialogueOptionClass, UUserWidget, "/Game/VN/WP_DialogueOption.WP_DialogueOption_C");
}

void UWidget_Dialogue::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClickToContinueBtn->OnClicked.AddDynamic(this,&ThisClass::OnClickToContinueBtnClicked);
}

void UWidget_Dialogue::NativeConstruct()
{
	Super::NativeConstruct();
	if (!IsValid(mDialogueContext))
	{
		return;
	}
	UpdateDialogue();
}

void UWidget_Dialogue::OnClickToContinueBtnClicked()
{
	int32 optionNum = mShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if(optionNum ==1)
	{
		if (mDialogueContext->ChooseOption(0))
		{
			UpdateDialogue();
		}
		else
		{
			RemoveFromParent();
		}
	}
}

void UWidget_Dialogue::UpdateDialogue()
{
	for (int32 i = 0; i < ButtonsVBox->GetChildrenCount();++i)
	{
		ButtonsVBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}

	DialogueText->SetText(mDialogueContext->GetActiveNodeText());
	UObject* activeParicipant =
		Cast<UObject>(mDialogueContext->GetActiveNodeParticipant());
	if (IsValid(activeParicipant))
	{
		FText speakerName =
			IDlgDialogueParticipant::Execute_GetParticipantDisplayName(activeParicipant, FName());
		SpeakerName->SetText(speakerName);
		SpeakerBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SpeakerBorder->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!IsValid(mDialogueOptionClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Dialogue::mDialogueOptionClass 클래스 없음"));
		return;
	}
	int32 optionNum = mShowUnselectableOption ? 
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if (optionNum < 2)
	{
		return;
	}
	for (int32 i = 0; i < optionNum; ++i)
	{
		UWidget_DialogueOption* dialogueOption = nullptr;
		bool canRecycle = i < ButtonsVBox->GetChildrenCount();
		if(canRecycle)
		{
			dialogueOption=Cast<UWidget_DialogueOption>(ButtonsVBox->GetChildAt(i));
		}
		else
		{
			dialogueOption =
				CreateWidget<UWidget_DialogueOption>(GetOwningPlayer(), mDialogueOptionClass);
		}
		if (!IsValid(dialogueOption))
		{
			UE_LOG(LogTemp, Warning, TEXT("UWidget_Dialogue::dialogueOption 캐스팅 실패"));
			return;
		}
		if(mShowUnselectableOption)
		{
			dialogueOption->Init(this,mDialogueContext->GetOptionTextFromAll(i),i);
		}
		else
		{
			dialogueOption->Init(this,mDialogueContext->GetOptionText(i),i);
		}
		dialogueOption->SetVisibility(ESlateVisibility::Visible);
		if (!canRecycle)
		{
			ButtonsVBox->AddChild(dialogueOption);
		}

		if(mShowUnselectableOption)
		{
			dialogueOption->SetIsEnabled(mDialogueContext->IsOptionSatisfied(i));
		}
	}
}

void UWidget_Dialogue::ChooseOption(int32 OptionIndex)
{
	if(mShowUnselectableOption)
	{
		mDialogueContext->ChooseOptionFromAll(OptionIndex);
	}
	else
	{
		mDialogueContext->ChooseOption(OptionIndex);
	}
	UpdateDialogue();
}

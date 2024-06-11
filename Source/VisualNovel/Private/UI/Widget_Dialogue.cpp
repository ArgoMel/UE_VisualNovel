#include "UI/Widget_Dialogue.h"
#include "UI/Widget_DialogueOption.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

UWidget_Dialogue::UWidget_Dialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mTextSpeed = 0.05f;
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
	UpdateText();
}

void UWidget_Dialogue::OnClickToContinueBtnClicked()
{
	if(mCurText!=mTargetText)
	{
		mTypeTimer.Invalidate();
		mCurText = mTargetText;
		DialogueText->SetText(FText::FromString(mCurText));
		ShowOptions();
		return;
	}

	int32 optionNum = mShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if(optionNum ==1)
	{
		if (mDialogueContext->ChooseOption(0))
		{
			UpdateText();
		}
		else
		{
			RemoveFromParent();
		}
	}
}

void UWidget_Dialogue::DelayTypeText()
{
	if(mConsumedText.IsEmpty()||
		mCurText == mTargetText)
	{
		mTypeTimer.Invalidate();
		ShowOptions();
		return;
	}
	mCurText += mConsumedText[0];
	mConsumedText=mConsumedText.RightChop(1);
	DialogueText->SetText(FText::FromString(mCurText));
}

void UWidget_Dialogue::UpdateText()
{
	for (int32 i = 0; i < ButtonsVBox->GetChildrenCount();++i)
	{
		ButtonsVBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}

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

	if(mTextSpeed==0.f)
	{
		mCurText = UBFL_VN::ToTargetText(
			mDialogueContext->GetActiveNodeText(),IsValid(activeParicipant));
		mTargetText= mCurText;
		DialogueText->SetText(FText::FromString(mCurText));
		ShowOptions();
	}
	else
	{
		mCurText.Empty();
		mConsumedText= UBFL_VN::ToTargetText(
			mDialogueContext->GetActiveNodeText(), IsValid(activeParicipant));
		mTargetText = mConsumedText;
		DialogueText->SetText(FText::GetEmpty());
		GetWorld()->GetTimerManager().SetTimer(mTypeTimer, this, &ThisClass::DelayTypeText, mTextSpeed, true);
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
	UpdateText();
}

void UWidget_Dialogue::ShowOptions()
{
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
		if (canRecycle)
		{
			dialogueOption = Cast<UWidget_DialogueOption>(ButtonsVBox->GetChildAt(i));
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
		FString tempString;
		if (mShowUnselectableOption)
		{
			tempString=UBFL_VN::ToTargetText(
				mDialogueContext->GetOptionTextFromAll(i), false);
		}
		else
		{
			tempString=UBFL_VN::ToTargetText(
				mDialogueContext->GetOptionText(i), false);
		}
		dialogueOption->Init(this, FText::FromString(tempString), i);
		dialogueOption->SetVisibility(ESlateVisibility::Visible);
		if (!canRecycle)
		{
			ButtonsVBox->AddChild(dialogueOption);
		}

		if (mShowUnselectableOption)
		{
			dialogueOption->SetIsEnabled(mDialogueContext->IsOptionSatisfied(i));
		}
	}
}

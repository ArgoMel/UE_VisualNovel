#include "UI/Widget_Dialogue.h"
#include "UI/Widget_DialogueOption.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Option.h"
#include "UI/Widget_Participant.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include <DlgSystem/DlgManager.h>
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Animation/WidgetAnimation.h"
#include <Components/CanvasPanel.h>

UWidget_Dialogue::UWidget_Dialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mTextSpeed = 0.05f;

	bShowUnselectableOption = true;
	bAskForPlayerName = false;

	GetClassAsset(mDialogueOptionClass, UUserWidget, "/Game/VN/UI/WP_DialogueOption.WP_DialogueOption_C");
}

void UWidget_Dialogue::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClickToContinueBtn->OnClicked.AddDynamic(this,&ThisClass::OnClickToContinueBtnClicked);
	PlayerNameInput->OnTextChanged.AddDynamic(this,&ThisClass::OnPlayerNameInputChanged);
	PlayerNameInput->OnTextCommitted.AddDynamic(this,&ThisClass::OnPlayerNameInputCommitted);

	FWidgetAnimationDynamicEvent finishedDelegate;
	finishedDelegate.BindDynamic(this, &ThisClass::CheckNotify);
	BindToAnimationFinished(Anim_Notify, finishedDelegate);
}

void UWidget_Dialogue::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(mDialogueContext))
	{
		UpdateText();
	}
}

FName UWidget_Dialogue::GetParticipantName_Implementation() const
{
	return TEXT("Widget");
}

bool UWidget_Dialogue::ModifyNameValue_Implementation(FName ValueName, 
	FName NameValue)
{
	if(ValueName== VALUENAME_NOTIFY)
	{
		Notify(FText::FromName(NameValue));
	}
	return false;
}

void UWidget_Dialogue::OnClickToContinueBtnClicked()
{
	if(PlayerNameInput->IsVisible())
	{
		return;
	}
	if(mCurText!=mTargetText)
	{
		mTypeTimer.Invalidate();
		mCurText = mTargetText;
		DialogueText->SetText(FText::FromString(mCurText));
		ShowOptions();
		return;
	}

	int32 optionNum = bShowUnselectableOption ?
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

void UWidget_Dialogue::OnPlayerNameInputChanged(const FText& Text)
{
	FString temp = Text.ToString();
	if(temp.Len()>=10)
	{
		PlayerNameInput->SetText(FText::FromString(temp.Left(10)));
	}
}

void UWidget_Dialogue::OnPlayerNameInputCommitted(const FText& Text, 
	ETextCommit::Type CommitMethod)
{
	if(CommitMethod!=ETextCommit::OnEnter||
		Text.IsEmpty())
	{
		return;
	}
	//AParticipant* participant=
	//	Cast<AParticipant>(mDialogueContext->GetMutableParticipant(ISPHERIA));
	//if(!IsValid(participant))
	//{
	//	return;
	//}
	//participant->mPlayerName=Text;
	PlayerNameInput->SetVisibility(ESlateVisibility::Collapsed);
	ChooseOption(0);
}

void UWidget_Dialogue::OnShowUnselectableOptionChecked(bool Value)
{
	bShowUnselectableOption = Value;
}

void UWidget_Dialogue::OnTextSpeedChanged(float Value)
{
	mTextSpeed = Value;
	UE_LOG(LogTemp, Log, TEXT("%f"), mTextSpeed);
}

void UWidget_Dialogue::DelayTypeText()
{
	if(mConsumedText.IsEmpty()||
		mCurText == mTargetText)
	{
		ShowOptions();
		return;
	}
	if (mCurText.EndsWith(TEXT(">")))
	{
		mCurText.RemoveFromEnd(TEXT("</>"));
	}
	bool isSymbol = true;
	TArray<FString> paths;
	mConsumedText.ParseIntoArray(paths, TEXT(">"));
	mConsumedText.Empty();
	for (FString& element : paths)
	{
		element += TEXT(">");
		if (isSymbol&&element.StartsWith(TEXT("<")))
		{
			mCurText += element;
		}
		else
		{
			isSymbol = false;
			mConsumedText += element;
		}
	}
	mCurText += mConsumedText[0];
	mCurText += TEXT("</>");
	mConsumedText=mConsumedText.RightChop(1);
	DialogueText->SetText(FText::FromString(mCurText));
}

void UWidget_Dialogue::CheckNotify()
{
	if (mNotificationQueue.IsEmpty())
	{
		return;
	}
	NotifyTextBlock->SetText(UBFL_VN::ToTargetText(mNotificationQueue[0]));
	mNotificationQueue.RemoveAt(0);
	PlayAnimation(Anim_Notify);
}

void UWidget_Dialogue::UpdateText()
{
	HideOptions();

	UObject* activeParicipant =
		Cast<UObject>(mDialogueContext->GetActiveNodeParticipant());
	FText speakerName = FText::GetEmpty();
	if (IsValid(activeParicipant))
	{
		speakerName =
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
		mCurText = UBFL_VN::ToTargetString(
			mDialogueContext->GetActiveNodeText(),IsValid(activeParicipant));
		mTargetText= mCurText;
		DialogueText->SetText(FText::FromString(mCurText));
		ShowOptions();
	}
	else
	{
		mCurText.Empty();
		mConsumedText= UBFL_VN::ToTargetString(
			mDialogueContext->GetActiveNodeText(), IsValid(activeParicipant));
		mTargetText = mConsumedText;
		DialogueText->SetText(FText::GetEmpty());
		GetWorld()->GetTimerManager().SetTimer(mTypeTimer, this, &ThisClass::DelayTypeText, mTextSpeed, true);
	}

	mMenuWidget->AddEntry(UBFL_VN::ToTargetText(speakerName), FText::FromString(mTargetText));
}

void UWidget_Dialogue::ChooseOption(int32 OptionIndex)
{
	if(bShowUnselectableOption)
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
	if(bAskForPlayerName)
	{
		PlayerNameInput->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	if(!mTypeTimer.IsValid())
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(mTypeTimer);
	if (!IsValid(mDialogueOptionClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Dialogue::mDialogueOptionClass 클래스 없음"));
		return;
	}
	int32 optionNum = bShowUnselectableOption ?
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
			continue;
		}
		FText tempText;
		if (bShowUnselectableOption)
		{
			tempText =UBFL_VN::ToTargetText(
				mDialogueContext->GetOptionTextFromAll(i), false);
		}
		else
		{
			tempText =UBFL_VN::ToTargetText(
				mDialogueContext->GetOptionText(i), false);
		}
		dialogueOption->Init(this, tempText, i);
		dialogueOption->SetVisibility(ESlateVisibility::Visible);
		if (!canRecycle)
		{
			ButtonsVBox->AddChild(dialogueOption);
		}

		if (bShowUnselectableOption)
		{
			dialogueOption->SetIsEnabled(mDialogueContext->IsOptionSatisfied(i));
		}
	}
}

void UWidget_Dialogue::HideOptions()
{
	for (int32 i = 0; i < ButtonsVBox->GetChildrenCount(); ++i)
	{
		ButtonsVBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
		ButtonsVBox->GetChildAt(i)->SetIsEnabled(true);
	}
}

void UWidget_Dialogue::Notify(FText NotifyText)
{
	mNotificationQueue.Add(NotifyText);
	if (IsAnimationPlaying(Anim_Notify))
	{
		return;
	}
	CheckNotify();
}

void UWidget_Dialogue::GetParticipants(UDlgDialogue* Dialogue, 
	TArray<UObject*>& Participants)
{
	TMap<FName, UObject*> participantMap = { {ParticipantName_Widget,this}};
	UCanvasPanel* canvas = Cast<UCanvasPanel>(GetRootWidget());
	for(auto& child:canvas->GetAllChildren())
	{
		UWidget_Participant* paricipant=Cast<UWidget_Participant>(child);
		if(IsValid(paricipant)) 
		{
			participantMap.Add(paricipant->GetParticipantName(), paricipant);
		}
	}
	
	for(auto& participantName: Dialogue->GetParticipantNames())
	{
		if(participantMap.Contains(participantName))
		{
			Participants.Add(participantMap.FindRef(participantName));
		}
	}
}

void UWidget_Dialogue::Init(UWidget_Menu* Menu, UDlgDialogue* Dialogue)
{
	mMenuWidget = Menu;
	if (IsValid(mMenuWidget))
	{
		mMenuWidget->GetOption()->OnShowUnselectableOptionChecked.AddDynamic(this,&ThisClass::OnShowUnselectableOptionChecked);
		mMenuWidget->GetOption()->OnTextSpeedChanged.AddDynamic(this,&ThisClass::OnTextSpeedChanged);
		mMenuWidget->GetOption()->ApplyOptionFirst(bShowUnselectableOption,mTextSpeed);
	}
	if(IsValid(Dialogue))
	{
		TArray<UObject*> participants;
		GetParticipants(Dialogue, participants);
		mDialogueContext = UDlgManager::StartDialogue(Dialogue, participants);
	}
}

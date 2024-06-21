#include "UI/Widget_Dialogue.h"
#include "UI/Widget_DialogueOption.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Option.h"
#include "UI/Widget_Participant.h"
#include "GameInstance/GI_VN.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include <DlgSystem/DlgManager.h>
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include <Components/CanvasPanel.h>
#include "Components/VerticalBoxSlot.h"
#include "Animation/WidgetAnimation.h"

UWidget_Dialogue::UWidget_Dialogue(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mTextSpeed = 0.05f;

	bShowUnselectableOption = true;
	bAskForPlayerName = false;
	bShowPreviouslyPickedChoices = true;

	mSkipSpeed = 0.001f;
	bSkipModeActive = false;
	bCancelSkipOnOptions = false;

	mAutoSpeed = 0.5f;
	bAutoModeActive = false;
	bCancelAutoOnOptions = false;

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
	BindToAnimationFinished(NotifyAnim, finishedDelegate);
}

void UWidget_Dialogue::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(mDialogueContext))
	{
		ChangeBG(TEXT("Modern-Dormroom1"));
		UpdateText();
	}
}

FName UWidget_Dialogue::GetParticipantName_Implementation() const
{
	return PARTICIPANTNAME_WIDGET;
}

bool UWidget_Dialogue::ModifyNameValue_Implementation(FName ValueName, 
	FName NameValue)
{
	if(ValueName== VALUENAME_NOTIFY)
	{
		Notify(FText::FromName(NameValue));
	}
	else if (ValueName == VALUENAME_CHANGE_BG_SLIDE)
	{
		ChangeBG(NameValue);
		PlayAnimation(SlideAnim);
	}
	else if (ValueName == VALUENAME_CHANGE_BG_FADE)
	{
		ChangeBG(NameValue);
		PlayAnimation(FadeAnim);
	}
	else if (ValueName == VALUENAME_CHANGE_BG_DESOLVE)
	{
		ChangeBG(NameValue);
		PlayAnimation(DesolveAnim);
	}
	else if (ValueName == VALUENAME_CHANGE_BG_WIPE)
	{
		ChangeBG(NameValue);
		PlayAnimation(WipeAnim);
	}
	return false;
}

void UWidget_Dialogue::OnClickToContinueBtnClicked()
{
	if(PlayerNameBorder->IsVisible())
	{
		return;
	}
	if(mCurText!=mTargetText)
	{
		GetWorld()->GetTimerManager().ClearTimer(mTypeTimer);
		mCurText = mTargetText;
		DialogueText->SetText(FText::FromString(mCurText));
		OnTextFinishedTyping();
		return;
	}

	int32 optionNum = bShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if(optionNum ==1)
	{
		for (auto& participant : mParticipants)
		{
			UWidget_Participant* participantWidget = Cast<UWidget_Participant>(participant.Value);
			if (!IsValid(participantWidget))
			{
				continue;
			}
			participantWidget->FinishAnimating();
		}
		StopAllAnimations();
		if (mDialogueContext->ChooseOption(0))
		{
			UpdateText();
		}
		else
		{
			mMenuWidget->OnMenuBtnClicked();
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

	UGI_VN* gameInstance = Cast<UGI_VN>(GetGameInstance());
	gameInstance->mPlayerName = FName(Text.ToString());
	PlayerNameBorder->SetVisibility(ESlateVisibility::Collapsed);
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
		OnTextFinishedTyping();
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
	NotifyTextBlock->SetText(UBFL_VN::ToTargetText(mNotificationQueue[0],false,true));
	mNotificationQueue.RemoveAt(0);
	PlayAnimation(NotifyAnim);
}

void UWidget_Dialogue::SkipTextTyping()
{
	if (bSkipModeActive)
	{
		ChooseOption();
	}
}

void UWidget_Dialogue::AutoTextTyping()
{
	if (bAutoModeActive)
	{
		ChooseOption();
	}
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

	if(mTextSpeed==0.f||
		bSkipModeActive)
	{
		mCurText = UBFL_VN::ToTargetString(
			mDialogueContext->GetActiveNodeText(),IsValid(activeParicipant));
		mTargetText= mCurText;
		DialogueText->SetText(FText::FromString(mCurText));
		OnTextFinishedTyping();
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
	for (auto& participant : mParticipants)
	{
		UWidget_Participant* participantWidget = Cast<UWidget_Participant>(participant.Value);
		if (!IsValid(participantWidget))
		{
			continue;
		}
		participantWidget->FinishAnimating();
	}
	StopAllAnimations();
	bool canChoose = false;
	if(bShowUnselectableOption)
	{
		canChoose =mDialogueContext->ChooseOptionFromAll(OptionIndex);
	}
	else
	{
		canChoose =mDialogueContext->ChooseOption(OptionIndex);
	}
	if(canChoose)
	{
		UpdateText();
	}
	else
	{
		mMenuWidget->OnMenuBtnClicked();
	}
}

void UWidget_Dialogue::OnTextFinishedTyping()
{
	if(bAskForPlayerName)
	{
		PlayerNameBorder->SetVisibility(ESlateVisibility::Visible);
		PlayerNameInput->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(mTypeTimer);
	int32 optionNum = bShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if (optionNum ==1)	//선택지가 없는경우
	{
		FTimerHandle tempTimer;
		if (bSkipModeActive)
		{
			bSkipModeActive =
				mDialogueContext->IsOptionConnectedToVisitedNode(0, false, !bShowUnselectableOption);
		}
		if(bSkipModeActive)
		{
			GetWorld()->GetTimerManager().SetTimer(tempTimer, this, &ThisClass::SkipTextTyping, mSkipSpeed);
		}		
		else if (bAutoModeActive)
		{
			float delay = mTextSpeed==0.f? 
				mDialogueContext->GetActiveNodeText().ToString().Len() * 0.01f : 
				mAutoSpeed;
			GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::AutoTextTyping, delay);
		}
		return;
	}
	if(bCancelSkipOnOptions)
	{
		bSkipModeActive = false;
	}
	if (bCancelAutoOnOptions)
	{
		bAutoModeActive = false;
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
			UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(dialogueOption->Slot);
			if (IsValid(slot))
			{
				slot->SetPadding(FMargin(0.f, 5.f, 0.f, 5.f));
			}
		}

		if (bShowUnselectableOption)
		{
			dialogueOption->SetIsEnabled(mDialogueContext->IsOptionSatisfied(i));
		}

		dialogueOption->SetPreviouslyPickedChoice(bShowPreviouslyPickedChoices, 
			mDialogueContext->IsOptionConnectedToVisitedNode(i, false, !bShowUnselectableOption));
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
	if (IsAnimationPlaying(NotifyAnim))
	{
		return;
	}
	CheckNotify();
}

void UWidget_Dialogue::StartDialogue(UDlgDialogue* Dialogue)
{
	if (IsValid(Dialogue))
	{
		TArray<UObject*> participants;
		GetParticipants(Dialogue, participants);
		mDialogueContext = UDlgManager::StartDialogue(Dialogue, participants);
	}
	else
	{
		mDialogueContext = nullptr;
	}
}

void UWidget_Dialogue::GetParticipants(UDlgDialogue* Dialogue, 
	TArray<UObject*>& Participants)
{
	if(mParticipants.IsEmpty())
	{
		mParticipants.Add(PARTICIPANTNAME_WIDGET, this);
		mParticipants.Add(PARTICIPANTNAME_GAMEINSTANCE, GetGameInstance());
		UCanvasPanel* canvas = Cast<UCanvasPanel>(GetRootWidget());
		for (auto& child : canvas->GetAllChildren())
		{
			UWidget_Participant* paricipant = Cast<UWidget_Participant>(child);
			if (IsValid(paricipant))
			{
				mParticipants.Add(paricipant->GetParticipantName(), paricipant);
			}
		}
	}

	for(auto& participantName: Dialogue->GetParticipantNames())
	{
		if(mParticipants.Contains(participantName))
		{
			Participants.Add(mParticipants.FindRef(participantName));
		}
	}
}

void UWidget_Dialogue::Init(UWidget_Menu* Menu)
{
	mMenuWidget = Menu;
}

void UWidget_Dialogue::ChangeBG(FName TextureName)
{
	FProgressBarStyle style = AnimPB->GetWidgetStyle();
	style.BackgroundImage.SetResourceObject(BGImg->GetBrush().GetResourceObject());
	style.FillImage.SetResourceObject(UBFL_VN::GetBGImgData(TextureName).Texture);
	AnimPB->SetWidgetStyle(style);
	BGImg->SetBrushFromTexture(UBFL_VN::GetBGImgData(TextureName).Texture);
}

void UWidget_Dialogue::SetDialogueVisible(bool Visible)
{
	ClickToContinueBtn->SetIsEnabled(Visible);
	for (int32 i = 0; i < ButtonsVBox->GetChildrenCount();++i)
	{
		ButtonsVBox->GetChildAt(i)->SetIsEnabled(Visible);
	}
	if(Visible)
	{
		PlayAnimationReverse(FadeDialogueAnim);
	}
	else
	{
		PlayAnimationForward(FadeDialogueAnim);
	}
}

void UWidget_Dialogue::ToggleSkipMode()
{
	bSkipModeActive = !bSkipModeActive;
	int32 optionNum = bShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if (bSkipModeActive&&
		mDialogueContext->IsOptionConnectedToVisitedNode(0, false, !bShowUnselectableOption)&&
		optionNum ==1)
	{
		ChooseOption();
	}
}

void UWidget_Dialogue::ToggleAutoMode()
{
	bAutoModeActive = !bAutoModeActive;
	int32 optionNum = bShowUnselectableOption ?
		mDialogueContext->GetAllOptionsNum() : mDialogueContext->GetOptionsNum();
	if (bAutoModeActive&&
		mCurText==mTargetText&&
		optionNum ==1)
	{
		ChooseOption();
	}
}

void UWidget_Dialogue::Ending()
{
	mDialogueContext = nullptr;
	Reset();
}

void UWidget_Dialogue::Reset()
{
	RemoveFromParent();
	bSkipModeActive = false;
	bAutoModeActive = false;
}

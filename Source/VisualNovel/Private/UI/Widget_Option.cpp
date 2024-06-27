#include "UI/Widget_Option.h"
#include "UI/Widget_Dialogue.h"
#include "Save/PersistantData.h"
#include "BFL/BFL_VN.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/EditableText.h"

UWidget_Option::UWidget_Option(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_Option::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ShowUnselectableOptionCB->OnCheckStateChanged.AddDynamic(this,&ThisClass::OnShowUnselectableOptionCBChecked);
	ShowPreviouslyPickedChoicesCB->OnCheckStateChanged.AddDynamic(this,&ThisClass::OnShowPreviouslyPickedChoicesCBChecked);
	TextSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnTextSpeedSliderChanged);

	SkipSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnSkipSpeedSliderChanged);
	CancelSkipModeOnOptionCB->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCancelSkipModeOnOptionCBChecked);

	AutoSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnAutoSpeedSliderChanged);
	CancelAutoModeOnOptionCB->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCancelAutoModeOnOptionCBChecked);

	MasterVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnMasterVolumeSliderChanged);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnMusicVolumeSliderChanged);
	VoiceVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnVoiceVolumeSliderChanged);
	SFXVolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnSFXVolumeSliderChanged);

	TextSizeEText->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextSizeETextCommitted);
}

void UWidget_Option::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_Option::OnShowUnselectableOptionCBChecked(bool Value)
{
	mPersistantData->bShowUnselectableOption = Value;
	mDialogueWidget->bShowUnselectableOption=Value;
}

void UWidget_Option::OnShowPreviouslyPickedChoicesCBChecked(bool Value)
{
	mPersistantData->bShowPreviouslyPickedChoices = Value;
	mDialogueWidget->bShowPreviouslyPickedChoices = Value;
}

void UWidget_Option::OnTextSpeedSliderChanged(float Value)
{
	mPersistantData->mTextSpeed = Value;
	mDialogueWidget->mTextSpeed = TextSpeedSlider->GetMaxValue() - Value;
	TextSpeedPB->SetPercent(Value);
}

void UWidget_Option::OnSkipSpeedSliderChanged(float Value)
{
	mPersistantData->mSkipSpeed = Value;
	mDialogueWidget->mSkipSpeed = FMath::Max(SkipSpeedSlider->GetMaxValue() - Value, 0.01f);
	SkipSpeedPB->SetPercent(Value);
}

void UWidget_Option::OnCancelSkipModeOnOptionCBChecked(bool Value)
{
	mPersistantData->bCancelSkipOnOptions = Value;
	mDialogueWidget->bCancelSkipOnOptions = Value;
}

void UWidget_Option::OnAutoSpeedSliderChanged(float Value)
{
	mPersistantData->mAutoSpeed = Value;
	mDialogueWidget->mAutoSpeed = FMath::Lerp(0.25f,1.f,AutoSpeedSlider->GetMaxValue() - Value);
	AutoSpeedPB->SetPercent(Value);
}

void UWidget_Option::OnCancelAutoModeOnOptionCBChecked(bool Value)
{
	mPersistantData->bCancelAutoOnOptions = Value;
	mDialogueWidget->bCancelAutoOnOptions = Value;
}

void UWidget_Option::OnMasterVolumeSliderChanged(float Value)
{
	mPersistantData->mVolumes[(int32)ESoundKind::Master] = Value;
	UBFL_VN::SetVolume(GetWorld(), Value, ESoundKind::Master);
	MasterVolumePB->SetPercent(Value);
}

void UWidget_Option::OnMusicVolumeSliderChanged(float Value)
{
	mPersistantData->mVolumes[(int32)ESoundKind::BGM] = Value;
	UBFL_VN::SetVolume(GetWorld(), Value, ESoundKind::BGM);
	MusicVolumePB->SetPercent(Value);
}

void UWidget_Option::OnVoiceVolumeSliderChanged(float Value)
{
	mPersistantData->mVolumes[(int32)ESoundKind::Voice] = Value;
	UBFL_VN::SetVolume(GetWorld(), Value, ESoundKind::Voice);
	VoiceVolumePB->SetPercent(Value);
}

void UWidget_Option::OnSFXVolumeSliderChanged(float Value)
{
	mPersistantData->mVolumes[(int32)ESoundKind::SFX] = Value;
	UBFL_VN::SetVolume(GetWorld(), Value, ESoundKind::SFX);
	SFXVolumePB->SetPercent(Value);
}

void UWidget_Option::OnTextSizeETextCommitted(const FText& Text, 
	ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter ||
		Text.IsEmpty())
	{
		TextSizeEText->SetText(FText::AsNumber(mPersistantData->mTextSize));
		return;
	}
	int32 size = FCString::Atoi(*Text.ToString());
	mDialogueWidget->mTextSize = size;
	mPersistantData->mTextSize = size;
	TextSizeEText->SetText(FText::AsNumber(size));
}

void UWidget_Option::InitializeSavedOptions()
{
	ShowUnselectableOptionCB->SetIsChecked(mPersistantData->bShowUnselectableOption);
	ShowPreviouslyPickedChoicesCB->SetIsChecked(mPersistantData->bShowPreviouslyPickedChoices);
	TextSpeedSlider->SetValue(mPersistantData->mTextSpeed);	
	SkipSpeedSlider->SetValue(mPersistantData->mSkipSpeed);
	CancelSkipModeOnOptionCB->SetIsChecked(mPersistantData->bCancelSkipOnOptions);
	AutoSpeedSlider->SetValue(mPersistantData->mAutoSpeed);
	CancelAutoModeOnOptionCB->SetIsChecked(mPersistantData->bCancelAutoOnOptions);
	MasterVolumeSlider->SetValue(mPersistantData->mVolumes[(int32)ESoundKind::Master]);
	MusicVolumeSlider->SetValue(mPersistantData->mVolumes[(int32)ESoundKind::BGM]);
	VoiceVolumeSlider->SetValue(mPersistantData->mVolumes[(int32)ESoundKind::Voice]);
	SFXVolumeSlider->SetValue(mPersistantData->mVolumes[(int32)ESoundKind::SFX]);
	mDialogueWidget->mTextSize = mPersistantData->mTextSize;
	TextSizeEText->SetText(FText::AsNumber(mDialogueWidget->mTextSize));
}

void UWidget_Option::Init(UWidget_Dialogue* DialogueWidget, 
	UPersistantData* PersistantData)
{
	mDialogueWidget = DialogueWidget;
	mPersistantData = PersistantData;
}
#include "UI/Widget_Option.h"
#include "UI/Widget_Dialogue.h"
#include "Save/PersistantData.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"

UWidget_Option::UWidget_Option(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_Option::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ShowUnselectableOptionCB->OnCheckStateChanged.AddDynamic(this,&ThisClass::OnShowUnselectableOptionCBChecked);
	TextSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnTextSpeedSliderChanged);

	SkipSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnSkipSpeedSliderChanged);
	CancelSkipModeOnOptionCB->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCancelSkipModeOnOptionCBChecked);

	AutoSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnAutoSpeedSliderChanged);
	CancelAutoModeOnOptionCB->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCancelAutoModeOnOptionCBChecked);
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

void UWidget_Option::InitializeSavedOptions()
{
	ShowUnselectableOptionCB->SetIsChecked(mPersistantData->bShowUnselectableOption);
	TextSpeedSlider->SetValue(mPersistantData->mTextSpeed);	
}

void UWidget_Option::Init(UWidget_Dialogue* DialogueWidget, 
	UPersistantData* PersistantData)
{
	mDialogueWidget = DialogueWidget;
	mPersistantData = PersistantData;
}

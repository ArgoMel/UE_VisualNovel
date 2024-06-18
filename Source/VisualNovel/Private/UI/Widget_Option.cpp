#include "UI/Widget_Option.h"
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
}

void UWidget_Option::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_Option::OnShowUnselectableOptionCBChecked(bool Value)
{
	mPersistantData->bShowUnselectableOption = Value;
	OnShowUnselectableOptionChecked.Broadcast(Value);
}

void UWidget_Option::OnTextSpeedSliderChanged(float Value)
{
	mPersistantData->mTextSpeed = Value;
	TextSpeedPB->SetPercent(Value);
	OnTextSpeedChanged.Broadcast(TextSpeedSlider->GetMaxValue()-Value);
}

void UWidget_Option::InitializeSavedOptions()
{
	ShowUnselectableOptionCB->SetIsChecked(mPersistantData->bShowUnselectableOption);
	TextSpeedSlider->SetValue(mPersistantData->mTextSpeed);	
}

void UWidget_Option::Init(UPersistantData* PersistantData)
{
	mPersistantData = PersistantData;
}

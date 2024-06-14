#include "UI/Widget_Option.h"
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
	OnShowUnselectableOptionCBChecked(ShowUnselectableOptionCB->GetIsEnabled());
	OnTextSpeedSliderChanged(TextSpeedSlider->GetValue());
}

void UWidget_Option::OnShowUnselectableOptionCBChecked(bool Value)
{
	OnShowUnselectableOptionChecked.Broadcast(Value);
}

void UWidget_Option::OnTextSpeedSliderChanged(float Value)
{
	TextSpeedPB->SetPercent(Value);
	OnTextSpeedChanged.Broadcast(TextSpeedSlider->GetMaxValue()-Value);
}

void UWidget_Option::ApplyOptionFirst(bool& ShowUnselectableOption, float& TextSpeed)
{
	ShowUnselectableOption = ShowUnselectableOptionCB->GetIsEnabled();
	TextSpeed = TextSpeedSlider->GetMaxValue()-TextSpeedSlider->GetValue();
}

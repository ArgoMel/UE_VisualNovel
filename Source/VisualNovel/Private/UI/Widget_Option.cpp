#include "UI/Widget_Option.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"

UWidget_Option::UWidget_Option(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_Option::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TextSpeedSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnTextSpeedSliderChanged);
}

void UWidget_Option::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_Option::OnTextSpeedSliderChanged(float Value)
{
	TextSpeedPB->SetPercent(Value);
}

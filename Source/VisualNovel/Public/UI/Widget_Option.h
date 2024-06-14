#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Option.generated.h"

class USlider;
class UProgressBar;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Option : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Option(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> TextSpeedSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> TextSpeedPB;

protected:
	UFUNCTION()
	void OnTextSpeedSliderChanged(float Value);
};

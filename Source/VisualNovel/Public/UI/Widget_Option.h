#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Option.generated.h"

class UCheckBox;
class USlider;
class UProgressBar;
class UPersistantData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowUnselectableOptionChecked,bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextSpeedChanged,float,Value);

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
	TObjectPtr<UCheckBox> ShowUnselectableOptionCB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> TextSpeedSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> TextSpeedPB;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnShowUnselectableOptionChecked OnShowUnselectableOptionChecked;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnTextSpeedChanged OnTextSpeedChanged;

protected:
	UFUNCTION()
	void OnShowUnselectableOptionCBChecked(bool Value);
	UFUNCTION()
	void OnTextSpeedSliderChanged(float Value);

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void InitializeSavedOptions();

	void Init(UPersistantData* PersistantData);
};

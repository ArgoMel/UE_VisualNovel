#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Option.generated.h"

class UCheckBox;
class USlider;
class UProgressBar;
class UEditableText;
class UWidget_Dialogue;
class UPersistantData;

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
	TObjectPtr<UCheckBox> ShowPreviouslyPickedChoicesCB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> TextSpeedSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> TextSpeedPB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> SkipSpeedSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> SkipSpeedPB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> CancelSkipModeOnOptionCB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> AutoSpeedSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> AutoSpeedPB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> CancelAutoModeOnOptionCB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> MasterVolumeSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> MasterVolumePB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> MusicVolumeSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> MusicVolumePB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> VoiceVolumeSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> VoiceVolumePB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> SFXVolumeSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> SFXVolumePB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableText> TextSizeEText;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;

protected:
	UFUNCTION()
	void OnShowUnselectableOptionCBChecked(bool Value);
	UFUNCTION()
	void OnShowPreviouslyPickedChoicesCBChecked(bool Value);
	UFUNCTION()
	void OnTextSpeedSliderChanged(float Value);
	UFUNCTION()
	void OnSkipSpeedSliderChanged(float Value);
	UFUNCTION()
	void OnCancelSkipModeOnOptionCBChecked(bool Value);
	UFUNCTION()
	void OnAutoSpeedSliderChanged(float Value);
	UFUNCTION()
	void OnCancelAutoModeOnOptionCBChecked(bool Value);
	UFUNCTION()
	void OnMasterVolumeSliderChanged(float Value);
	UFUNCTION()
	void OnMusicVolumeSliderChanged(float Value);
	UFUNCTION()
	void OnVoiceVolumeSliderChanged(float Value);
	UFUNCTION()
	void OnSFXVolumeSliderChanged(float Value);
	UFUNCTION()
	void OnTextSizeETextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void InitializeSavedOptions();

	void Init(UWidget_Dialogue* DialogueWidget,UPersistantData* PersistantData);
};

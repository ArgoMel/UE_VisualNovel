#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Dialogue.generated.h"

class UBorder;
class UTextBlock;
class UVerticalBox;
class UButton;
class UDlgContext;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Dialogue : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Dialogue(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	FTimerHandle mTypeTimer;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> SpeakerBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeakerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonsVBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ClickToContinueBtn;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueOptionClass;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mCurText;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mConsumedText;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mTargetText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	float mTextSpeed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	bool mShowUnselectableOption;

public:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Ref", meta = (ExposeOnSpawn = true))
	TObjectPtr<UDlgContext> mDialogueContext;

protected:
	UFUNCTION()
	void OnClickToContinueBtnClicked();

	UFUNCTION()
	void DelayTypeText();

public:
	void UpdateText();
	void ChooseOption(int32 OptionIndex);
	void ShowOptions();
};

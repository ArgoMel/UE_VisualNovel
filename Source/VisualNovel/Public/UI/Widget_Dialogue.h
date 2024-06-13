#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Widget_Dialogue.generated.h"

class URichTextBlock;
class UBorder;
class UTextBlock;
class UVerticalBox;
class UButton;
class UEditableText;
class UDlgContext;
class UDlgDialogue;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Dialogue : public UUserWidget
	,public IDlgDialogueParticipant
{
	GENERATED_BODY()
public:
	UWidget_Dialogue(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	FName GetParticipantName_Implementation() const;
	bool ModifyNameValue_Implementation(FName ValueName, FName NameValue);

private:
	FTimerHandle mTypeTimer;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> DialogueText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> SpeakerBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeakerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> NotifyTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonsVBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ClickToContinueBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableText> PlayerNameInput;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Anim_Notify;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UDlgContext> mDialogueContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueOptionClass;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FText> mNotificationQueue;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mCurText;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mConsumedText;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mTargetText;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	float mTextSpeed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	bool bShowUnselectableOption;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	bool bAskForPlayerName;

public:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Ref", meta = (ExposeOnSpawn = true))
	TArray<UObject*> mParticipants;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Ref", meta = (ExposeOnSpawn = true))
	TObjectPtr<UDlgDialogue> mDLGDialogue;

protected:
	UFUNCTION()
	void OnClickToContinueBtnClicked();
	UFUNCTION()
	void OnPlayerNameInputChanged(const FText& Text);
	UFUNCTION()
	void OnPlayerNameInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void DelayTypeText();
	UFUNCTION()
	void CheckNotify();

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateText();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ChooseOption(int32 OptionIndex=0);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowOptions();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void Notify(FText NotifyText);
};

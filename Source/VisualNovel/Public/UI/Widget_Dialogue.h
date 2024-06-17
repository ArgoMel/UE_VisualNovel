#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Widget_Dialogue.generated.h"

class UImage;
class URichTextBlock;
class UBorder;
class UTextBlock;
class UVerticalBox;
class UButton;
class UEditableText;
class UWidget_Menu;
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
	TObjectPtr<UImage> BGImg;

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
	TMap<FName, UObject*> mParticipants;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Ref", meta = (ExposeOnSpawn = true))
	TObjectPtr<UWidget_Menu> mMenuWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UDlgContext> mDialogueContext;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueOptionClass;

	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mCurText;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mConsumedText;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mTargetText;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	float mTextSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FText> mNotificationQueue;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FText mPlayerName;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowUnselectableOption;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bAskForPlayerName;

protected:
	UFUNCTION()
	void OnClickToContinueBtnClicked();
	UFUNCTION()
	void OnPlayerNameInputChanged(const FText& Text);
	UFUNCTION()
	void OnPlayerNameInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnShowUnselectableOptionChecked(bool Value);
	UFUNCTION()
	void OnTextSpeedChanged(float Value);

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
	void HideOptions();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void Notify(FText NotifyText);

	UFUNCTION(BlueprintPure, Category = "Default")
	void GetParticipants(UDlgDialogue* Dialogue, TArray<UObject*>& Participants);

	void Init(UWidget_Menu* Menu,UDlgDialogue* Dialogue);

	UDlgContext* GetDialogueContext()
	{
		return mDialogueContext;
	}
};

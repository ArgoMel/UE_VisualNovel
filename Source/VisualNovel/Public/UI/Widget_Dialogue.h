#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interface_VNSave.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Widget_Dialogue.generated.h"

class UImage;
class UProgressBar;
class URichTextBlock;
class UBorder;
class UTextBlock;
class UVerticalBox;
class UButton;
class UEditableText;
class UWidget_Menu;
class UDlgContext;
class UDlgDialogue;
class UPersistantData;
class UGI_VN;
class AGM_VN;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Dialogue : public UUserWidget
	,public IDlgDialogueParticipant
	, public IInterface_VNSave
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
public:
	void OnNewGame_Implementation();
	void OnSaveGame_Implementation(USG_VN* SaveGame);
	void OnLoadGame_Implementation(USG_VN* SaveGame);

private:
	FTimerHandle mTypeTimer;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BGImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> AnimPB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> DialogueBorder;
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
	TObjectPtr<UBorder> PlayerNameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableText> PlayerNameInput;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> NotifyAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SlideAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> DesolveAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> WipeAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeDialogueAnim;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TMap<FName, UObject*> mParticipants;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Menu> mMenuWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UDlgContext> mDialogueContext;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UGI_VN> mGameInstance;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<AGM_VN> mGameMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueOptionClass;

	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mCurText;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mConsumedText;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	FString mTargetText;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FText> mNotificationQueue;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bAskForPlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	bool bSkipModeActive;

	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	bool bAutoModeActive;

	UPROPERTY(BlueprintReadWrite, Category = "VNSave")
	FName mCurDialogueName;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	int32 mTextSize;
	UPROPERTY(BlueprintReadWrite, Category = "UpdateText")
	float mTextSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowUnselectableOption;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	bool bShowPreviouslyPickedChoices;

	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	float mSkipSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Skip")
	bool bCancelSkipOnOptions;

	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	float mAutoSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Auto")
	bool bCancelAutoOnOptions;

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
	UFUNCTION()
	void SkipTextTyping();
	UFUNCTION()
	void AutoTextTyping();

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateText();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ChooseOption(int32 OptionIndex=0);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnTextFinishedTyping();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void HideOptions();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void Notify(FText NotifyText);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void StartDialogue(UDlgDialogue* Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void PlayVoice();

	UFUNCTION(BlueprintPure, Category = "Default")
	void GetParticipants(UDlgDialogue* Dialogue, TArray<UObject*>& Participants);

	void Init(UWidget_Menu* Menu, UPersistantData* PersistantData);
	void ChangeBG(FName TextureName);
	void SetDialogueVisible(bool Visible);
	void ToggleSkipMode();
	void ToggleAutoMode();
	void Ending();
	void Reset(bool ResetDialogue=false);
	void Resume();

	FName GetDialogueName() const
	{
		return mCurDialogueName;
	}
};

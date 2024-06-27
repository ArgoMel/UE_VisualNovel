#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Menu.generated.h"

class UImage;
class UButton;
class UWidgetSwitcher;
class UWidget_History;
class UWidget_Option;
class UWidget_Codex;
class UWidget_Gallery;
class UWidget_SaveLoad;
class UWidget_Dialogue;
class UPersistantData;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Menu : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Menu(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	int32 mNextWigetIndex;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BGImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartBtn;	//시작 메뉴에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> HistoryBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CodexBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SkipBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> AutoBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ReplayBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> LoadBtn;	//항상 존재
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QSaveBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QLoadBtn;	//항상 존재
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> MenuBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> OptionBtn;	//항상 존재
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> GalleryBtn;	//시작 메뉴에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitBtn;	//시작 메뉴에만

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuWS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_History> History;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_Option> Option;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_Codex> Codex;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_SaveLoad> SaveLoad;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_Gallery> Gallery;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeBtnAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeBlackAnim;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;

protected:
	UFUNCTION()
	void OnStartBtnClicked();
	UFUNCTION()
	void OnHistoryBtnClicked();
	UFUNCTION()
	void OnCodexBtnClicked();
	UFUNCTION()
	void OnSkipBtnClicked();
	UFUNCTION()
	void OnAutoBtnClicked();
	UFUNCTION()
	void OnReplayBtnClicked();
	UFUNCTION()
	void OnSaveBtnClicked();
	UFUNCTION()
	void OnLoadBtnClicked();
	UFUNCTION()
	void OnQSaveBtnClicked();
	UFUNCTION()
	void OnQLoadBtnClicked();
	UFUNCTION()
	void OnMenuBtnClicked();
	UFUNCTION()
	void OnOptionBtnClicked();
	UFUNCTION()
	void OnGalleryBtnClicked();
	UFUNCTION()
	void OnBackBtnClicked();
	UFUNCTION()
	void OnQuitBtnClicked();

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void OpenMenu();
	UFUNCTION()
	void Credit();
	UFUNCTION()
	void QuitGame();

	bool SwitchWidget(int32 Index,bool IsForced=false);

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateButtonVisibility();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ToggleMenuWidget();

	UFUNCTION(BlueprintPure)
	bool IsInGame();

	void Init(UWidget_Dialogue* DialogueWidget, UPersistantData* PersistantData);
	void AddEntry(FText Name, FText EntryText);
	void ChangeScene(int32 Index);
	void UpdateGallery(FString TextureName);
	void Save(FString SlotName,bool WillGameExit = false);
	void Load(FString SlotName);
	void ToggleForScreenshot(bool TurnAllWidget=false);
	void PlayCredit(bool CanSkip=false);

	UWidget_Option* GetOption()
	{
		return Option;
	}
	UWidget_Codex* GetCodex()
	{
		return Codex;
	}
};

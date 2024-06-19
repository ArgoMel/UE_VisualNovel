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
class UWidget_Dialogue;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Menu : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Menu(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

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
	TObjectPtr<UButton> SaveBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> LoadBtn;	//항상 존재
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuickSaveBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuickLoadBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> MenuBtn;	// 게임 중에만
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> OptionBtn;	//항상 존재
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
	void OnMenuBtnClicked();
	UFUNCTION()
	void OnOptionBtnClicked();
	UFUNCTION()
	void OnBackBtnClicked();
	UFUNCTION()
	void OnQuitBtnClicked();

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateButtonVisibility();

	UFUNCTION(BlueprintPure)
	bool IsInGame();

	void Init(UWidget_Dialogue* DialogueWidget);
	void AddEntry(FText Name, FText EntryText);

	UWidget_Option* GetOption()
	{
		return Option;
	}
	UWidget_Codex* GetCodex()
	{
		return Codex;
	}
};

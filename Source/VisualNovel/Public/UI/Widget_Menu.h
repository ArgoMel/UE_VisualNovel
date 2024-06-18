#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Menu.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget_History;
class UWidget_Option;
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
	TObjectPtr<UButton> StartBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> LoadBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CodexBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> HistoryBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> MenuBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> OptionBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuWS;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_History> History;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget_Option> Option;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;

protected:
	UFUNCTION()
	void OnStartBtnClicked();
	UFUNCTION()
	void OnHistoryBtnClicked();
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
};

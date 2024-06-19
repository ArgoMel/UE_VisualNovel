#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Codex.generated.h"

class UScrollBox;
class UTextBlock;
class UWidget_CodexBtn;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Codex : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Codex(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScrollBox> BtnSB;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScrollBox> DetailSB;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mCodexBtnClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mCodexDetailClass;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_CodexBtn> mRecentCodexBtn;

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void CreateCodexButtons();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowCodexDetail(UDlgDialogue* Codex);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void AddToCodexDetail(UDlgContext* CodexContext,int32 Index=0);

	void UpdateText();
};

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

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_CodexBtn> mLastClickedBtn;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mCodexBtnClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mCodexDetailClass;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TMap<FName, UWidget_CodexBtn*> mCodexBtns;

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void CreateCodexButtons(UWidget_Dialogue* Dialogue);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void ShowCodexDetail(FText CodexName,TArray<FText> CodexDetails, 
		UWidget_CodexBtn* CodexBtn);

	void UpdateRecentCodexDetail();
};

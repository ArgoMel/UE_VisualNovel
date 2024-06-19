#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_CodexBtn.generated.h"

class UButton;
class UTextBlock;
class UWidget_Codex;
class UDlgDialogue;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_CodexBtn : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_CodexBtn(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CodexBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CodexText;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Codex> mCodexWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UDlgDialogue> mCodexEntry;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FText> mCodexDetails;

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateCodexDetails();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void AddToCodexDetail(UDlgContext* CodexContext);

	UFUNCTION()
	void OnCodexBtnClicked();

	void Init(UWidget_Codex* Codex, UDlgDialogue* Dialogue);

	FText GetCodexText();
};

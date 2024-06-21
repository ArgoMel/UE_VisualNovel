#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_DialogueOption.generated.h"

class UButton;
class URichTextBlock;
class UCheckBox;
class UWidget_Dialogue;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_DialogueOption : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_DialogueOption(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> DialogueBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> DialogueText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCheckBox> DialogueCB;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Ref", meta = (ExposeOnSpawn = true))
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Variable", meta = (ExposeOnSpawn = true))
	int32 mOptionIndex;

protected:
	UFUNCTION()
	void OnDialogueBtnClicked();

public:
	void Init(UWidget_Dialogue* DialogueContext,FText Text,int32 OptionIndex);
	void SetPreviouslyPickedChoice(bool UseCB,bool WasChoice);
};

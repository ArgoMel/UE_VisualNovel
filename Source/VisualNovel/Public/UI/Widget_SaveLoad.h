#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_SaveLoad.generated.h"

class UTextBlock;
class UUniformGridPanel;
class UWidget_Menu;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_SaveLoad : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_SaveLoad(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SaveText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SaveContainer;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Menu> mMenuWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mSaveBtnClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Variable")
	int32 mSavesPerRow;
	UPROPERTY(BlueprintReadOnly, Category = "Variable")
	bool bIsSaveMode;

protected:
	UFUNCTION()
	void OnSaveOrLoad(FString SlotName);

	UFUNCTION()
	void SetSaveButton(UWidget_SaveBtn* SaveBtn, FString Name);

public:
	void CreateSaveButtons(UWidget_Menu* Menu);
	void SetSaving(bool IsSaving);
	void SetScreenshotIndex(FString Name);

	bool GetIsSaveMode() const
	{
		return bIsSaveMode;
	}
};

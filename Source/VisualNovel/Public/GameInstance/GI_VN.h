#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_VN.generated.h"

class UPersistantData;
class UWidget_Menu;
class UWidget_Option;
class UWidget_Dialogue;

UCLASS()
class VISUALNOVEL_API UGI_VN : public UGameInstance
{
	GENERATED_BODY()
public:
	UGI_VN();
protected:
	virtual void Init() override;
	virtual void Shutdown() override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Menu> mMenuWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Option> mOptionWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mMenuWidgetClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueWidgetClass;

public:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ShowMenu();
};

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "GI_VN.generated.h"

class UPersistantData;
class UWidget_Menu;
class UWidget_Option;
class UWidget_Dialogue;

UCLASS()
class VISUALNOVEL_API UGI_VN : public UGameInstance
	, public IDlgDialogueParticipant
{
	GENERATED_BODY()
public:
	UGI_VN();
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
public:
	FName GetParticipantName_Implementation() const;
	bool CheckCondition_Implementation(const UDlgContext* Context, FName ConditionName) const;
	bool OnDialogueEvent_Implementation(UDlgContext* Context, FName EventName);
	bool ModifyNameValue_Implementation(FName ValueName, FName NameValue);

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

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FName> mTriggeredFlags;

public:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ShowMenu();
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ShowDialogue();

	void CreateMenu();

	UWidget_Menu* GetMenuWidget()
	{
		return mMenuWidget;
	}
	UWidget_Dialogue* GetDialogueWidget()
	{
		return mDialogueWidget;
	}
};

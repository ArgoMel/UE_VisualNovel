#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Interface/Interface_VNSave.h"
#include "GI_VN.generated.h"

class UUserWidget;
class UPersistantData;
class UWidget_Menu;
class UWidget_Option;
class UWidget_Dialogue;

UCLASS()
class VISUALNOVEL_API UGI_VN : public UGameInstance
	, public IDlgDialogueParticipant
	, public IInterface_VNSave
{
	GENERATED_BODY()
public:
	UGI_VN();
	virtual void Init() override;
	virtual void Shutdown() override;
public:
	virtual FName GetParticipantName_Implementation() const override;
	virtual bool CheckCondition_Implementation(const UDlgContext* Context, FName ConditionName) const override;
	virtual bool ModifyNameValue_Implementation(FName ValueName, FName NameValue) override;
public:
	virtual void OnNewGame_Implementation() override;
	virtual void OnSaveGame_Implementation(USG_VN* SaveGame) override;
	virtual void OnLoadGame_Implementation(USG_VN* SaveGame) override;

private:
	int32 mLoadedAssetCount;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Menu> mMenuWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_Dialogue> mDialogueWidget;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UUserWidget> mLoadingWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mMenuWidgetClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mDialogueWidgetClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mLoadingWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TSet<FName> mTriggeredFlags;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAssetLoadComplete);
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnAssetLoadComplete OnAssetLoadComplete;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FText mPlayerName;

protected:
	UFUNCTION()
	void LoadAssetComplete();

public:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ShowMenu() const;
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ResumeDialogue() const;

	void CreateUI();
	void ToggleGameAndMenu() const;
	void ChangeVNSaveData(FString OldName,FString NewName) const;
	void ShowLoading(bool IsShow) const;
	bool IsAllAssetLoading() const;

	UWidget_Menu* GetMenuWidget()
	{
		return mMenuWidget;
	}
	UWidget_Dialogue* GetDialogueWidget()
	{
		return mDialogueWidget;
	}
};

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_SaveBtn.generated.h"

class UButton;
class UImage;
class UEditableText;
class UTextBlock;
class UGI_VN;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_SaveBtn : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_SaveBtn(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void SetIsEnabled(bool bInIsEnabled) override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SaveImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableText> SaveName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SaveDetail;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UGI_VN> mGameInstance;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveOrLoad,FString, SlotName);
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnSaveOrLoad OnSaveOrLoad;

	FString mSlotName;

protected:
	UFUNCTION()
	void OnSaveBtnClicked();
	UFUNCTION()
	void OnSaveNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	void SetSaveImg(UTexture2D* Texture);
	void SetSaveImgFromObj(UObject* Texture);
	void SetSaveName(FString Text);
	void SetSaveDetail(FText Text);
};

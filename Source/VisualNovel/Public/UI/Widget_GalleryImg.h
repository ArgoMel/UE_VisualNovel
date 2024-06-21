#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GalleryImg.generated.h"

class UImage;
class UButton;
class APC_VN;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_GalleryImg : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_GalleryImg(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GalleryImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> PrevImgBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> NextImgBtn;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<APC_VN> mVNPC;

	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FString> mImageCode;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mImageName;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	int32 mCurIndex;

private:
	UFUNCTION()
	void HideWidget();
	void ChangeImage();

protected:
	UFUNCTION()
	void OnPrevImgBtnClicked();
	UFUNCTION()
	void OnNextImgBtnClicked();

public:
	void SetImage(FString ImageName, TArray<FString>& ImageCode);
};

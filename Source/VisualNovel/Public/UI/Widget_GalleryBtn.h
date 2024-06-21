#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GalleryBtn.generated.h"

class UButton;
class UWidget_GalleryImg;
class UPersistantData;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_GalleryBtn : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_GalleryBtn(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> GalleryBtn;

	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UWidget_GalleryImg> mGalleryImg;
	UPROPERTY(BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UPersistantData> mPersistantData;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly ,Category = "Variable")
	TObjectPtr<UTexture2D> mBlackTex;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FString mImageName;
	/*
		단순히 이미지 개수로 하면 선택지 분기에 따라 
		000, 002만 저장될수도 있어서 배열로 함
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	TArray<FString> mImageCode;

protected:
	UFUNCTION()
	void OnGalleryBtnClicked();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateButton();

	void Init(UWidget_GalleryImg* GalleryImg,UPersistantData* PersistantData,
		FString ImageName, TArray<FString>& ImageCode);

	FString GetImageName() const
	{
		return mImageName;
	}
	void GetImageCode(TArray<FString>& ImageCode) const
	{
		ImageCode = mImageCode;
	}
};

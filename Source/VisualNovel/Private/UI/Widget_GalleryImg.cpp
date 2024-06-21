#include "UI/Widget_GalleryImg.h"
#include "PlayerController/PC_VN.h"
#include "../VisualNovel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include <Engine/AssetManager.h>

UWidget_GalleryImg::UWidget_GalleryImg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mCurIndex = 0;
}

void UWidget_GalleryImg::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PrevImgBtn->OnClicked.AddDynamic(this, &ThisClass::OnPrevImgBtnClicked);
	NextImgBtn->OnClicked.AddDynamic(this, &ThisClass::OnNextImgBtnClicked);
	mVNPC =Cast<APC_VN>(GetOwningPlayer());
	if(IsValid(mVNPC))
	{
		mVNPC->OnMouseRightClicked.AddDynamic(this,&ThisClass::HideWidget);
	}
}

void UWidget_GalleryImg::HideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	mCurIndex = 0;
}

void UWidget_GalleryImg::ChangeImage()
{
	FString newName = mImageName +TEXT("_") + mImageCode[mCurIndex];
	UAssetManager& manager = UAssetManager::Get();
	FPrimaryAssetId asset = FPrimaryAssetId(PRIMARY_ASSET_TYPE_GALLERY, FName(newName));
	GalleryImg->SetBrushFromTexture(Cast<UTexture2D>(manager.GetPrimaryAssetObject(asset)));
}

void UWidget_GalleryImg::OnPrevImgBtnClicked()
{
	if(mImageCode.IsEmpty())
	{
		HideWidget();
		return;
	}
	--mCurIndex;
	if(mCurIndex<0)
	{
		HideWidget();
		return;
	}
	ChangeImage();
}

void UWidget_GalleryImg::OnNextImgBtnClicked()
{
	if (mImageCode.IsEmpty())
	{
		HideWidget();
		return;
	}
	++mCurIndex;
	if (mCurIndex >=mImageCode.Num())
	{
		HideWidget();
		return;
	}
	ChangeImage();
}

void UWidget_GalleryImg::SetImage(FString ImageName, TArray<FString>& ImageCode)
{
	mCurIndex = 0;
	mImageName = ImageName;
	mImageCode = ImageCode;
	if (mImageCode.IsEmpty())
	{
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	ChangeImage();
}

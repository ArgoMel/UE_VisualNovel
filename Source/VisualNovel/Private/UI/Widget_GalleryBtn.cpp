#include "UI/Widget_GalleryBtn.h"
#include "UI/Widget_GalleryImg.h"
#include "Save/PersistantData.h"
#include "../VisualNovel.h"
#include "Components/Button.h"
#include <Engine/AssetManager.h>

UWidget_GalleryBtn::UWidget_GalleryBtn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetObjectAsset(mBlackTex, UTexture2D, "/Game/VN/Texture/Etc/TX_Black.TX_Black");
}

void UWidget_GalleryBtn::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GalleryBtn->OnClicked.AddDynamic(this,&ThisClass::OnGalleryBtnClicked);
}

void UWidget_GalleryBtn::OnGalleryBtnClicked()
{
	mGalleryImg->SetImage(mImageName,mPersistantData->mUnlockedGalleryImgs.Find(mImageName)->Strings);
}

void UWidget_GalleryBtn::UpdateButton()
{
	FButtonStyle stype = GalleryBtn->GetStyle();
	if(mPersistantData->mUnlockedGalleryImgs.Contains(mImageName))
	{
		UAssetManager& manager = UAssetManager::Get();
		FPrimaryAssetId asset = FPrimaryAssetId(PRIMARY_ASSET_TYPE_GALLERY, 
			FName(mImageName + TEXT("_") + mImageCode[0]));
		UObject* galleryTex = manager.GetPrimaryAssetObject(asset);
		stype.Normal.SetResourceObject(galleryTex);
		stype.Pressed.SetResourceObject(galleryTex);
		stype.Hovered.SetResourceObject(galleryTex);
		GalleryBtn->SetIsEnabled(true);
	}
	else
	{
		stype.Normal.SetResourceObject(mBlackTex);
		stype.Pressed.SetResourceObject(mBlackTex);
		stype.Hovered.SetResourceObject(mBlackTex);
		GalleryBtn->SetIsEnabled(false);
	}
	GalleryBtn->SetStyle(stype);
}

void UWidget_GalleryBtn::Init(UWidget_GalleryImg* GalleryImg,
	UPersistantData* PersistantData, FString ImageName, TArray<FString>& ImageCode)
{
	mGalleryImg = GalleryImg;
	mPersistantData = PersistantData;
	mImageName = ImageName;
	mImageCode = ImageCode;
}
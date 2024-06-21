#include "UI/Widget_Gallery.h"
#include "UI/Widget_GalleryImg.h"
#include "UI/Widget_GalleryBtn.h"
#include "../VisualNovel.h"
#include <Engine/AssetManager.h>
#include "Components/UniformGridPanel.h"

UWidget_Gallery::UWidget_Gallery(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mGalleryBtnClass, UUserWidget, "/Game/VN/UI/WP_GalleryBtn.WP_GalleryBtn_C");
	GetClassAsset(mGalleryImgClass, UUserWidget, "/Game/VN/UI/WP_GalleryImg.WP_GalleryImg_C");

	mImagesPerRow = 4;
}

void UWidget_Gallery::CreateGalleryMenu(UPersistantData* PersistantData)
{
	if (!IsValid(mGalleryImgClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"),
			*GetFName().ToString(), *mGalleryImgClass->GetFName().ToString());
		return;
	}
	UWidget_GalleryImg* galleryImg =
		CreateWidget<UWidget_GalleryImg>(GetOwningPlayer(), mGalleryImgClass);
	galleryImg->AddToViewport(2);

	UAssetManager& manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> outAssets;
	manager.GetPrimaryAssetIdList(PRIMARY_ASSET_TYPE_GALLERY, outAssets);

	if (!IsValid(mGalleryBtnClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"),
			*GetFName().ToString(), *mGalleryBtnClass->GetFName().ToString());
		return;
	}
	for (int32 i = 0; i<outAssets.Num();++i)
	{
		UWidget_GalleryBtn* galleryBtn =
			CreateWidget<UWidget_GalleryBtn>(GetOwningPlayer(), mGalleryBtnClass);

		TArray<FString> imageCode;
		FString assetName;
		FString assetCode;
		outAssets[i].PrimaryAssetName.ToString().Split(
			TEXT("_"), &assetName, &assetCode, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		imageCode.Add(assetCode);
		for (int32 j=1; j +i < outAssets.Num();++j)
		{
			FString nextName;
			outAssets[i + j].PrimaryAssetName.ToString().Split(
				TEXT("_"), &nextName, &assetCode,ESearchCase::IgnoreCase,ESearchDir::FromEnd);
			if(nextName!= assetName)
			{
				break;
			}
			imageCode.Add(assetCode);
		}

		galleryBtn->Init(galleryImg,PersistantData, assetName, imageCode);
		i += imageCode.Num() - 1;
		galleryBtn->UpdateButton();
		int32 childNum = GalleryUGP->GetChildrenCount();
		GalleryUGP->AddChildToUniformGrid(galleryBtn, childNum / mImagesPerRow, childNum % mImagesPerRow);
	}
}

void UWidget_Gallery::UpdateGallery(FString TextureName)
{
	for (int32 i = 0; i < GalleryUGP->GetChildrenCount();++i)
	{
		UWidget_GalleryBtn* galleryBtn = 
			Cast<UWidget_GalleryBtn>(GalleryUGP->GetChildAt(i));
		if(galleryBtn->GetImageName()== TextureName)
		{
			galleryBtn->UpdateButton();
			break;
		}
	}
}

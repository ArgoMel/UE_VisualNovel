#include "UI/Widget_SaveLoad.h"
#include "UI/Widget_SaveBtn.h"
#include "UI/Widget_Menu.h"
#include "Save/SG_VN.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include <ImageUtils.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

UWidget_SaveLoad::UWidget_SaveLoad(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mSaveBtnClass, UUserWidget, "/Game/VN/UI/WP_SaveBtn.WP_SaveBtn_C");

	mSavesPerRow = 4;
	bIsSaveMode = true;
}

void UWidget_SaveLoad::OnSaveOrLoad(FString SlotName)
{
	if(bIsSaveMode)
	{
		mMenuWidget->Save(SlotName);
	}
	else
	{
		mMenuWidget->Load(SlotName);
	}
}

void UWidget_SaveLoad::SetSaveButton(UWidget_SaveBtn* SaveBtn, FString Name)
{
	//FString saveFile =
	//	FPaths::ProjectSavedDir() + TEXT("Screenshots/WindowsEditor/") + saveBtn->mSlotName + TEXT(".png");
	//saveBtn->SetSaveImg(FImageUtils::ImportFileAsTexture2D(saveFile));

	USG_VN* data = Cast<USG_VN>(UGameplayStatics::LoadGameFromSlot(Name, 0));
	if (data)
	{
		UAssetManager& manager = UAssetManager::Get();
		FPrimaryAssetId asset = FPrimaryAssetId(PRIMARY_ASSET_TYPE_GALLERY, data->mCurBG);
		UObject* galleryTex = manager.GetPrimaryAssetObject(asset);
		SaveBtn->SetSaveImgFromObj(galleryTex);
		SaveBtn->SetSaveDetail(FText::AsDateTime(data->mSaveTime));
	}
	SaveBtn->SetSaveName(Name);
}

void UWidget_SaveLoad::CreateSaveButtons(UWidget_Menu* Menu)
{
	mMenuWidget = Menu;
	if (!IsValid(mSaveBtnClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"),
			*GetFName().ToString(), *mSaveBtnClass->GetFName().ToString());
		return;
	}

	TArray<FString> names;
	UBFL_VN::GetAllSaveGameSlotNames(names);
	for (auto& name: names)
	{
		UWidget_SaveBtn* saveBtn =
			CreateWidget<UWidget_SaveBtn>(GetOwningPlayer(), mSaveBtnClass);
		if (!IsValid(saveBtn))
		{
			continue;
		}
		SetSaveButton(saveBtn, name);
		saveBtn->OnSaveOrLoad.AddDynamic(this,&ThisClass::OnSaveOrLoad);
		int32 childNum = SaveContainer->GetChildrenCount();
		SaveContainer->AddChildToUniformGrid(saveBtn, childNum / mSavesPerRow, childNum % mSavesPerRow);
	}
}

void UWidget_SaveLoad::SetSaving(bool IsSaving)
{
	bIsSaveMode = IsSaving;
	if(IsSaving)
	{
		SaveText->SetText(FText::FromString(TEXT("저장")));
	}
	else
	{
		SaveText->SetText(FText::FromString(TEXT("불러오기")));
	}
	for (int32 i = 0; i < SaveContainer->GetChildrenCount(); ++i)
	{
		SaveContainer->GetChildAt(i)->SetIsEnabled(IsSaving);
	}
}

void UWidget_SaveLoad::SetScreenshotIndex(FString Name)
{
	for (int32 i = 0; i < SaveContainer->GetChildrenCount();++i)
	{
		UWidget_SaveBtn* saveBtn =
			Cast<UWidget_SaveBtn>(SaveContainer->GetChildAt(i));
		if(saveBtn&&
			saveBtn->mSlotName==Name)
		{
			SetSaveButton(saveBtn, saveBtn->mSlotName);
			break;
		}
	}
}


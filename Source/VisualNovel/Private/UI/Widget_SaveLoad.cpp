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

UWidget_SaveLoad::UWidget_SaveLoad(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mSaveSlotIndex = 0;

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

void UWidget_SaveLoad::ScreenshotComplete()
{
	UWidget_SaveBtn* saveBtn =
		Cast<UWidget_SaveBtn>(SaveContainer->GetChildAt(mSaveSlotIndex));
	FString saveFile =
		FPaths::ProjectSavedDir() + TEXT("Screenshots/WindowsEditor/") + saveBtn->mSlotName + TEXT(".png");
	saveBtn->SetSaveImg(FImageUtils::ImportFileAsTexture2D(saveFile));
	USG_VN* data = Cast<USG_VN>(UGameplayStatics::LoadGameFromSlot(saveBtn->mSlotName, 0));
	saveBtn->SetSaveDetail(FText::AsDateTime(data->mSaveTime));
	saveBtn->SetSaveName(saveBtn->mSlotName);
	mMenuWidget->ToggleForScreenshot(saveBtn->mSlotName == SLOTNAME_QUICK_SAVE);
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
		FString saveFile=
			FPaths::ProjectSavedDir() + TEXT("Screenshots/WindowsEditor/") + name + TEXT(".png");
		if (FPaths::FileExists(saveFile))
		{
			saveBtn->SetSaveImg(FImageUtils::ImportFileAsTexture2D(saveFile));
			USG_VN* data = Cast<USG_VN>(UGameplayStatics::LoadGameFromSlot(name, 0));
			saveBtn->SetSaveDetail(FText::AsDateTime(data->mSaveTime));
		}
		saveBtn->SetSaveName(name);
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
		if(saveBtn->mSlotName==Name)
		{
			mSaveSlotIndex = i;
			FTimerHandle tempTimer;
			GetWorld()->GetTimerManager().SetTimer(tempTimer,this,&ThisClass::ScreenshotComplete,0.2f);
			break;
		}
	}

}


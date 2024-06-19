#include "UI/Widget_Codex.h"
#include "UI/Widget_CodexBtn.h"
#include "UI/Widget_CodexDetail.h"
#include "UI/Widget_Dialogue.h"
#include "GameInstance/GI_VN.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include "DlgSystem/DlgDialogue.h"
#include <DlgSystem/DlgManager.h>
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include <Engine/AssetManager.h>

UWidget_Codex::UWidget_Codex(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mCodexBtnClass, UUserWidget, "/Game/VN/UI/WP_CodexBtn.WP_CodexBtn_C");
	GetClassAsset(mCodexDetailClass, UUserWidget, "/Game/VN/UI/WP_CodexDetail.WP_CodexDetail_C");
}

void UWidget_Codex::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidget_Codex::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_Codex::CreateCodexButtons()
{
	UAssetManager& manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> outAssets;
	manager.GetPrimaryAssetIdList(PRIMARY_ASSET_TYPE_CODEX, outAssets);

	for (auto& asset : outAssets)
	{
		UDlgDialogue* dialogue = Cast<UDlgDialogue>(manager.GetPrimaryAssetObject(asset));
		if (!IsValid(dialogue))
		{
			continue;
		}
		if (!IsValid(mCodexBtnClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"), 
				*GetFName().ToString(),*mCodexBtnClass->GetFName().ToString());
			return;
		}
		UWidget_CodexBtn* codexBtn = 
			CreateWidget<UWidget_CodexBtn>(GetOwningPlayer(), mCodexBtnClass);
		if (!IsValid(codexBtn))
		{
			continue;
		}
		codexBtn->Init(this,dialogue);
		codexBtn->UpdateCodexDetails();
		BtnSB->AddChild(codexBtn);
		mCodexBtns.Add(asset.PrimaryAssetName, codexBtn);
	}
}

void UWidget_Codex::ShowCodexDetail(FText CodexName, TArray<FText> CodexDetails, 
	UWidget_CodexBtn* CodexBtn)
{
	TitleText->SetText(CodexName);
	mLastClickedBtn = CodexBtn;
	int32 index = 0;
	for(auto& codexDetail:CodexDetails)
	{
		UWidget_CodexDetail* codexDetailWidget = nullptr;
		if(index >= DetailSB->GetChildrenCount())
		{
			if (!IsValid(mCodexDetailClass))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"),
					*GetFName().ToString(), *mCodexDetailClass->GetFName().ToString());
				return;
			}
			codexDetailWidget =
				CreateWidget<UWidget_CodexDetail>(GetOwningPlayer(), mCodexDetailClass);
			DetailSB->AddChild(codexDetailWidget);
		}
		else
		{
			codexDetailWidget =
				Cast<UWidget_CodexDetail>(DetailSB->GetChildAt(index));
		}
		codexDetailWidget->Init(codexDetail);
		codexDetailWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		++index;
	}
	for (int32 i = CodexDetails.Num(); i < DetailSB->GetChildrenCount();++i)
	{
		DetailSB->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidget_Codex::UpdateRecentCodexDetail()
{
	if(!IsValid(mLastClickedBtn))
	{
		return;
	}
	mLastClickedBtn->OnCodexBtnClicked();
}
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
	UpdateText();
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
		BtnSB->AddChild(codexBtn);
	}
}

void UWidget_Codex::ShowCodexDetail(UDlgDialogue* Codex)
{
	UGI_VN* gameInstance = Cast<UGI_VN>(GetGameInstance());
	if (!IsValid(gameInstance))
	{
		return;
	}
	TArray<UObject*> participants;
	gameInstance->GetDialogueWidget()->GetParticipants(Codex, participants);
	UDlgContext* context = UDlgManager::StartDialogue(Codex, participants);
	TitleText->SetText(context->GetActiveNodeText());
	AddToCodexDetail(context,0);
}

void UWidget_Codex::AddToCodexDetail(UDlgContext* CodexContext, int32 Index)
{
	if(!CodexContext->ChooseOption(0))
	{
		for(int32 i=Index;i< DetailSB->GetChildrenCount();++i)
		{
			DetailSB->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
		}
		return;
	}
	UWidget_CodexDetail* codexDetail = nullptr;
	if(Index>=DetailSB->GetChildrenCount())
	{
		if (!IsValid(mCodexDetailClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s 클래스 없음"),
				*GetFName().ToString(), *mCodexDetailClass->GetFName().ToString());
			return;
		}
		codexDetail=
			CreateWidget<UWidget_CodexDetail>(GetOwningPlayer(), mCodexDetailClass);
		DetailSB->AddChild(codexDetail);
	}
	else
	{
		codexDetail =
			Cast<UWidget_CodexDetail>(DetailSB->GetChildAt(Index));
	}
	codexDetail->SetVisibility(ESlateVisibility::HitTestInvisible);
	codexDetail->Init(UBFL_VN::ToTargetText(CodexContext->GetActiveNodeText()));
	AddToCodexDetail(CodexContext, Index + 1);
}

void UWidget_Codex::UpdateText()
{
	UAssetManager& manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> outAssets;
	manager.GetPrimaryAssetIdList(PRIMARY_ASSET_TYPE_CODEX, outAssets);

	int32 index = 0;
	for (auto& asset : outAssets)
	{
		UDlgDialogue* dialogue = Cast<UDlgDialogue>(manager.GetPrimaryAssetObject(asset));
		if (!IsValid(dialogue))
		{
			continue;
		}
		UGI_VN* gameInstance = Cast<UGI_VN>(GetGameInstance());
		if (!IsValid(gameInstance))
		{
			continue;
		}
		TArray<UObject*> participants;
		gameInstance->GetDialogueWidget()->GetParticipants(dialogue, participants);
		UDlgContext* context = UDlgManager::StartDialogue(dialogue, participants);
		UWidget_CodexBtn* codexBtn =
			Cast<UWidget_CodexBtn>(BtnSB->GetChildAt(index));
		if (!IsValid(codexBtn))
		{
			continue;
		}
		codexBtn->SetText(context->GetActiveNodeText());
		if(codexBtn==mRecentCodexBtn)
		{
			ShowCodexDetail(dialogue);
		}
		++index;
	}
}

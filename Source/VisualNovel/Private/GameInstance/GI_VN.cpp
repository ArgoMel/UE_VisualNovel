#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_CodexBtn.h"
#include "Save/PersistantData.h"
#include "Save/SG_VN.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

UGI_VN::UGI_VN()
{
	mLoadedAssetCount = 0;

	GetClassAsset(mMenuWidgetClass, UUserWidget, "/Game/VN/UI/WP_Menu.WP_Menu_C");
	GetClassAsset(mDialogueWidgetClass, UUserWidget, "/Game/VN/UI/WP_Dialogue.WP_Dialogue_C");
	GetClassAsset(mLoadingWidgetClass, UUserWidget, "/Game/VN/UI/WP_Loading.WP_Loading_C");
}

void UGI_VN::Init()
{
	Super::Init();
	if (UGameplayStatics::DoesSaveGameExist(SLOTNAME_PERSISTANTDATA, 0))
	{
		mPersistantData = Cast<UPersistantData>(
			UGameplayStatics::LoadGameFromSlot(SLOTNAME_PERSISTANTDATA, 0));
	}
	else
	{
		mPersistantData = Cast<UPersistantData>(
			UGameplayStatics::CreateSaveGameObject(UPersistantData::StaticClass()));
		UGameplayStatics::SaveGameToSlot(mPersistantData, SLOTNAME_PERSISTANTDATA, 0);
	}
}

void UGI_VN::Shutdown()
{
	Super::Shutdown();
	if(!IsValid(mMenuWidget))
	{
		return;
	}
	mMenuWidget->Save(SLOTNAME_QUICK_SAVE,true);
	UGameplayStatics::SaveGameToSlot(mPersistantData, SLOTNAME_PERSISTANTDATA,0);
}

FName UGI_VN::GetParticipantName_Implementation() const
{
	return PARTICIPANTNAME_GAMEINSTANCE;
}

bool UGI_VN::CheckCondition_Implementation(const UDlgContext* Context, 
	FName ConditionName) const
{
	return mTriggeredFlags.Contains(ConditionName);
}

bool UGI_VN::ModifyNameValue_Implementation(FName ValueName, FName NameValue)
{
	if(ValueName.ToString().StartsWith(TEXT("CDX_")))
	{
		if(!mTriggeredFlags.Contains(NameValue))
		{
			mTriggeredFlags.Add(NameValue);
			UWidget_Codex* codex = mMenuWidget->GetCodex();
			UWidget_CodexBtn* codexBtn = codex->mCodexBtns.FindRef(ValueName);
			codexBtn->UpdateCodexDetails();
			FString notify = FString::Printf(TEXT("%s 정보 갱신!"), *codexBtn->GetCodexText().ToString());
			mDialogueWidget->Notify(FText::FromString(notify));
		}
	}
	else if (ValueName== VALUENAME_LOADLEVEL)
	{
		UGameplayStatics::OpenLevel(GetWorld(), NameValue);
	}
	return false;
}

void UGI_VN::OnNewGame_Implementation()
{
	mTriggeredFlags.Empty();
	mPlayerName = FText::FromString(PARTICIPANTNAME_DEFAULT);
}

void UGI_VN::OnSaveGame_Implementation(USG_VN* SaveGame)
{
	SaveGame->mTriggeredFlags= mTriggeredFlags;
	SaveGame->mPlayerName = mPlayerName.ToString();
	SaveGame->mSaveTime = FDateTime::UtcNow();
	IInterface_VNSave::Execute_OnSaveGame(mDialogueWidget, SaveGame);	
}

void UGI_VN::OnLoadGame_Implementation(USG_VN* SaveGame)
{
	mTriggeredFlags = SaveGame->mTriggeredFlags;
	mPlayerName = FText::FromString(SaveGame->mPlayerName);
	IInterface_VNSave::Execute_OnLoadGame(mDialogueWidget, SaveGame);
}

void UGI_VN::LoadAssetComplete()
{
	++mLoadedAssetCount;
	if(IsAllAssetLoading())
	{
		mMenuWidget->Init(mDialogueWidget, mPersistantData);
		UGameplayStatics::SetBaseSoundMix(GetWorld(), UBFL_VN::GetVNSoundMix());
		for (int32 i = 0; i < (int32)ESoundKind::Max; ++i)
		{
			UBFL_VN::SetVolume(GetWorld(), mPersistantData->mVolumes[i], (ESoundKind)i);
		}
	}
	OnAssetLoadComplete.Broadcast();
}

void UGI_VN::ShowMenu()
{
	mMenuWidget->AddToViewport(1);
}

void UGI_VN::ResumeDialogue()
{
	mDialogueWidget->AddToViewport(0);
	mDialogueWidget->Resume();
	ShowMenu();
}

void UGI_VN::CreateUI()
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(mDialogueWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mDialogueWidgetClass 클래스 없음"));
		return;
	}
	if (IsValid(mDialogueWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mDialogueWidget 이미 생성"));
		return;
	}
	mDialogueWidget = CreateWidget<UWidget_Dialogue>(pc, mDialogueWidgetClass);

	if (!IsValid(mMenuWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mMenuWidgetClass 클래스 없음"));
		return;
	}
	if (IsValid(mMenuWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mMenuWidget 이미 생성"));
		return;
	}
	mMenuWidget = CreateWidget<UWidget_Menu>(pc, mMenuWidgetClass);

	if (!IsValid(mLoadingWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mLoadingWidgetClass 클래스 없음"));
		return;
	}
	if (IsValid(mLoadingWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGI_VN::mLoadingWidget 이미 생성"));
		return;
	}
	mLoadingWidget = CreateWidget<UUserWidget>(pc, mLoadingWidgetClass);

	UAssetManager& manager = UAssetManager::Get();
	FStreamableDelegate loadCompleteDelegate;
	loadCompleteDelegate.BindUObject(this,&ThisClass::LoadAssetComplete);
	manager.LoadPrimaryAssetsWithType(
		PRIMARY_ASSET_TYPE_CODEX, TArray<FName>(), loadCompleteDelegate);
	manager.LoadPrimaryAssetsWithType(
		PRIMARY_ASSET_TYPE_GALLERY, TArray<FName>(), loadCompleteDelegate);
	manager.LoadPrimaryAssetsWithType(
		PRIMARY_ASSET_TYPE_SCRIPT, TArray<FName>(), loadCompleteDelegate);
	manager.LoadPrimaryAssetsWithType(
		PRIMARY_ASSET_TYPE_MUSIC, TArray<FName>(), loadCompleteDelegate);
	manager.LoadPrimaryAssetsWithType(
		PRIMARY_ASSET_TYPE_SOUNDCLASS, TArray<FName>(), loadCompleteDelegate);
}

void UGI_VN::ToggleGameAndMenu()
{
	mMenuWidget->ToggleMenuWidget();
}

void UGI_VN::ChangeVNSaveData(FString OldName, FString NewName)
{
	USG_VN* oldData = Cast<USG_VN>(
		UGameplayStatics::LoadGameFromSlot(OldName, 0));
	UGameplayStatics::SaveGameToSlot(oldData, NewName, 0);
	UGameplayStatics::DeleteGameInSlot(OldName, 0);
	int32 index=mPersistantData->mSaveNames.Find(OldName);
	mPersistantData->mSaveNames[index] = NewName;
}

void UGI_VN::ShowLoading(bool IsShow)
{ 
	if(IsShow)
	{
		mLoadingWidget->AddToViewport(0);
	}
	else
	{
		mLoadingWidget->RemoveFromParent();
	}
}

bool UGI_VN::IsAllAssetLoading() const
{
	UAssetManager& manager = UAssetManager::Get();
	TArray<FPrimaryAssetTypeInfo> infos;
	manager.GetPrimaryAssetTypeInfoList(infos);
	return mLoadedAssetCount >= (infos.Num()-2);
}

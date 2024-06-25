#include "GameInstance/GI_VN.h"
#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_CodexBtn.h"
#include "Save/PersistantData.h"
#include "Save/SG_VN.h"
#include "../VisualNovel.h"
#include <Kismet/GameplayStatics.h>

UGI_VN::UGI_VN()
{
	GetClassAsset(mMenuWidgetClass, UUserWidget, "/Game/VN/UI/WP_Menu.WP_Menu_C");
	GetClassAsset(mDialogueWidgetClass, UUserWidget, "/Game/VN/UI/WP_Dialogue.WP_Dialogue_C");
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
	return false;
}

void UGI_VN::OnNewGame_Implementation()
{
	mTriggeredFlags.Empty();
	mPlayerName= PARTICIPANTNAME_DEFAULT;
}

void UGI_VN::OnSaveGame_Implementation(USG_VN* SaveGame)
{
	SaveGame->mTriggeredFlags= mTriggeredFlags;
	SaveGame->mPlayerName = mPlayerName;
	SaveGame->mSaveTime = FDateTime::UtcNow();
	IInterface_VNSave::Execute_OnSaveGame(mDialogueWidget, SaveGame);	
}

void UGI_VN::OnLoadGame_Implementation(USG_VN* SaveGame)
{
	mTriggeredFlags = SaveGame->mTriggeredFlags;
	mPlayerName = SaveGame->mPlayerName;
	IInterface_VNSave::Execute_OnLoadGame(mDialogueWidget, SaveGame);
}

void UGI_VN::ShowMenu()
{
	mMenuWidget->AddToViewport(1);
}

void UGI_VN::ShowDialogue()
{
	mDialogueWidget->AddToViewport(0);
}

void UGI_VN::CreateMenu()
{
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
	mDialogueWidget = CreateWidget<UWidget_Dialogue>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0), mDialogueWidgetClass);

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
	mMenuWidget = CreateWidget<UWidget_Menu>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0), mMenuWidgetClass);
	mMenuWidget->Init(mDialogueWidget,mPersistantData);
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

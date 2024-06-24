#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_History.h"
#include "UI/Widget_Option.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_Gallery.h"
#include "Save/SG_VN.h"
#include "Interface/Interface_VNSave.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Animation/WidgetAnimation.h"
#include <Engine/AssetManager.h>
#include <Kismet/GameplayStatics.h>

UWidget_Menu::UWidget_Menu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	mNextWigetIndex = 0;
}

void UWidget_Menu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	StartBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartBtnClicked);
	HistoryBtn->OnClicked.AddDynamic(this, &ThisClass::OnHistoryBtnClicked);
	CodexBtn->OnClicked.AddDynamic(this, &ThisClass::OnCodexBtnClicked);
	SkipBtn->OnClicked.AddDynamic(this, &ThisClass::OnSkipBtnClicked);
	AutoBtn->OnClicked.AddDynamic(this, &ThisClass::OnAutoBtnClicked);
	SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	QSaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnQSaveBtnClicked);
	QLoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnQLoadBtnClicked);
	MenuBtn->OnClicked.AddDynamic(this, &ThisClass::OnMenuBtnClicked);
	OptionBtn->OnClicked.AddDynamic(this, &ThisClass::OnOptionBtnClicked);
	GalleryBtn->OnClicked.AddDynamic(this, &ThisClass::OnGalleryBtnClicked);
	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	QuitBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitBtnClicked);
}

void UWidget_Menu::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateButtonVisibility();
}

void UWidget_Menu::OnStartBtnClicked()
{
	IInterface_VNSave::Execute_OnNewGame(GetGameInstance());
	IInterface_VNSave::Execute_OnNewGame(History);
	IInterface_VNSave::Execute_OnNewGame(mDialogueWidget);
	Codex->UpdateAllCodex();
	ChangeScene(0);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer,this,
		&ThisClass::StartGame, FadeBlackAnim->GetEndTime() * 0.5f);
}

void UWidget_Menu::OnHistoryBtnClicked()
{
	SwitchWidget(1);
}

void UWidget_Menu::OnCodexBtnClicked()
{
	if(SwitchWidget(3))
	{
		Codex->UpdateRecentCodexDetail();
	}
}

void UWidget_Menu::OnSkipBtnClicked()
{
	mDialogueWidget->ToggleSkipMode();
}

void UWidget_Menu::OnAutoBtnClicked()
{
	mDialogueWidget->ToggleAutoMode();
}

void UWidget_Menu::OnQSaveBtnClicked()
{
	USG_VN* saveData = Cast<USG_VN>(
		UGameplayStatics::CreateSaveGameObject(USG_VN::StaticClass()));
	IInterface_VNSave::Execute_OnSaveGame(GetGameInstance(), saveData);
	IInterface_VNSave::Execute_OnSaveGame(History, saveData);
	UGameplayStatics::SaveGameToSlot(saveData, SLOTNAME_QUICK_SAVE, 0);
}

void UWidget_Menu::OnQLoadBtnClicked()
{
	if (!UGameplayStatics::DoesSaveGameExist(SLOTNAME_QUICK_SAVE, 0))
	{
		return;
	}
	USG_VN* saveData = Cast<USG_VN>(
		UGameplayStatics::LoadGameFromSlot(SLOTNAME_QUICK_SAVE, 0));
	IInterface_VNSave::Execute_OnLoadGame(GetGameInstance(), saveData);
	IInterface_VNSave::Execute_OnLoadGame(History, saveData);
	Codex->UpdateAllCodex();
}

void UWidget_Menu::OnMenuBtnClicked()
{
	ChangeScene(0);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this,
		&ThisClass::OpenMenu, FadeBlackAnim->GetEndTime() * 0.5f);
}

void UWidget_Menu::OnOptionBtnClicked()
{
	SwitchWidget(2);
}

void UWidget_Menu::OnGalleryBtnClicked()
{
	SwitchWidget(4);
}

void UWidget_Menu::OnBackBtnClicked()
{
	MenuWS->SetActiveWidgetIndex(0);
	if (IsInGame())
	{
		mDialogueWidget->Resume();
	}
	UpdateButtonVisibility();
}

void UWidget_Menu::OnQuitBtnClicked()
{
	ChangeScene(0);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this,
		&ThisClass::QuitGame, FadeBlackAnim->GetEndTime() * 0.5f);
}

void UWidget_Menu::StartGame()
{
	MenuWS->SetActiveWidgetIndex(mNextWigetIndex);
	UAssetManager& manager = UAssetManager::Get();
	FPrimaryAssetId asset = FPrimaryAssetId(PRIMARY_ASSET_TYPE_SCRIPT, VN_START_SCRIPT);
	UDlgDialogue* dialogue = Cast<UDlgDialogue>(manager.GetPrimaryAssetObject(asset));
	mDialogueWidget->StartDialogue(dialogue);
	mDialogueWidget->AddToViewport(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OpenMenu()
{
	MenuWS->SetActiveWidgetIndex(mNextWigetIndex);
	mDialogueWidget->Ending();
	UpdateButtonVisibility();
}

void UWidget_Menu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

bool UWidget_Menu::SwitchWidget(int32 Index)
{
	if (MenuWS->GetActiveWidgetIndex() == Index)
	{
		OnBackBtnClicked();
		return false;
	}
	MenuWS->SetActiveWidgetIndex(Index);
	if (IsValid(mDialogueWidget))
	{
		mDialogueWidget->Reset();
	}
	UpdateButtonVisibility();
	return true;
}

void UWidget_Menu::UpdateButtonVisibility()
{
	bool isInGame = IsInGame();
	if(isInGame)
	{
		StartBtn->SetVisibility(ESlateVisibility::Collapsed);
		HistoryBtn->SetVisibility(ESlateVisibility::Visible);
		CodexBtn->SetVisibility(ESlateVisibility::Visible);
		SkipBtn->SetVisibility(ESlateVisibility::Visible);
		AutoBtn->SetVisibility(ESlateVisibility::Visible);
		SaveBtn->SetVisibility(ESlateVisibility::Visible);
		QSaveBtn->SetVisibility(ESlateVisibility::Visible);
		MenuBtn->SetVisibility(ESlateVisibility::Visible);
		GalleryBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		StartBtn->SetVisibility(ESlateVisibility::Visible);
		HistoryBtn->SetVisibility(ESlateVisibility::Collapsed);
		CodexBtn->SetVisibility(ESlateVisibility::Collapsed);
		SkipBtn->SetVisibility(ESlateVisibility::Collapsed);
		AutoBtn->SetVisibility(ESlateVisibility::Collapsed);
		SaveBtn->SetVisibility(ESlateVisibility::Collapsed);
		QSaveBtn->SetVisibility(ESlateVisibility::Collapsed);
		MenuBtn->SetVisibility(ESlateVisibility::Collapsed);
		GalleryBtn->SetVisibility(ESlateVisibility::Visible);
	}
	if(MenuWS->GetActiveWidgetIndex()!=0)
	{
		BackBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BackBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (isInGame&& MenuWS->GetActiveWidgetIndex() == 0)
	{
		BGImg->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		BGImg->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWidget_Menu::ToggleMenuWidget()
{
	if (!IsInGame()|| 
		MenuWS->GetActiveWidgetIndex() != 0)
	{
		return;
	}
	if (IsInViewport())
	{
		PlayAnimationForward(FadeBtnAnim);
		FTimerHandle fadeTimer;
		GetWorld()->GetTimerManager().SetTimer(fadeTimer, this,
			&ThisClass::RemoveFromParent, FadeBtnAnim->GetEndTime());
		mDialogueWidget->SetDialogueVisible(false);
	}
	else
	{
		AddToViewport(1);
		PlayAnimationReverse(FadeBtnAnim);
		mDialogueWidget->SetDialogueVisible(true);
	}
}

bool UWidget_Menu::IsInGame()
{
	if(!IsValid(mDialogueWidget))
	{
		return false;
	}
	return !mDialogueWidget->GetDialogueName().IsNone();
}

void UWidget_Menu::Init(UWidget_Dialogue* DialogueWidget, 
	UPersistantData* PersistantData)
{
	mDialogueWidget = DialogueWidget;
	mDialogueWidget->Init(this, PersistantData);
	Option->Init(mDialogueWidget, PersistantData);
	Option->InitializeSavedOptions();
	Codex->CreateCodexButtons(mDialogueWidget);
	Gallery->CreateGalleryMenu(PersistantData);
}

void UWidget_Menu::AddEntry(FText Name, FText EntryText)
{
	History->AddEntry(Name, EntryText);
}

void UWidget_Menu::ChangeScene(int32 Index)
{
	mNextWigetIndex = Index;
	PlayAnimationForward(FadeBlackAnim);
}

void UWidget_Menu::UpdateGallery(FString TextureName)
{
	Gallery->UpdateGallery(TextureName);
}

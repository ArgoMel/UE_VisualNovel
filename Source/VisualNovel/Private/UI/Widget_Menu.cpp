#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_History.h"
#include "UI/Widget_Option.h"
#include "UI/Widget_Codex.h"
#include "UI/Widget_Gallery.h"
#include "UI/Widget_SaveLoad.h"
#include "Save/SG_VN.h"
#include "Interface/Interface_VNSave.h"
#include "BFL/BFL_VN.h"
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
	SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnSaveBtnClicked);
	LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnLoadBtnClicked);
	QSaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnQSaveBtnClicked);
	QLoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnQLoadBtnClicked);
	MenuBtn->OnClicked.AddDynamic(this, &ThisClass::OnMenuBtnClicked);
	OptionBtn->OnClicked.AddDynamic(this, &ThisClass::OnOptionBtnClicked);
	ReplayBtn->OnClicked.AddDynamic(this, &ThisClass::OnReplayBtnClicked);
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
	ChangeScene(0);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer,this,
		&ThisClass::StartGame, FadeBlackAnim->GetEndTime() * 0.5f);
	IInterface_VNSave::Execute_OnNewGame(GetGameInstance());
	IInterface_VNSave::Execute_OnNewGame(History);
	Codex->UpdateAllCodex();
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

void UWidget_Menu::OnReplayBtnClicked()
{
	mDialogueWidget->PlayVoice();
}

void UWidget_Menu::OnSaveBtnClicked()
{
	if (MenuWS->GetActiveWidgetIndex() == 5&&
		SaveLoad->GetIsSaveMode())
	{
		OnBackBtnClicked();
		return;
	}
	SwitchWidget(5,true);
	SaveLoad->SetSaving(true);
}

void UWidget_Menu::OnLoadBtnClicked()
{
	if (MenuWS->GetActiveWidgetIndex() == 5 &&
		!SaveLoad->GetIsSaveMode())
	{
		OnBackBtnClicked();
		return;
	}
	SwitchWidget(5, true);
	SaveLoad->SetSaving(false);
}

void UWidget_Menu::OnQSaveBtnClicked()
{
	Save(SLOTNAME_QUICK_SAVE);
}

void UWidget_Menu::OnQLoadBtnClicked()
{
	Load(SLOTNAME_QUICK_SAVE);
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
	mDialogueWidget->ChangeBG(VN_START_BG);
	mDialogueWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UpdateButtonVisibility();
}

void UWidget_Menu::OpenMenu()
{
	MenuWS->SetActiveWidgetIndex(mNextWigetIndex);
	mDialogueWidget->Reset(true);
	UpdateButtonVisibility();
}

void UWidget_Menu::Credit()
{
	mNextWigetIndex = 0;
	OpenMenu();
	//MenuWS->SetActiveWidgetIndex(mNextWigetIndex);
	//mDialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
	//UpdateButtonVisibility();
}

void UWidget_Menu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

bool UWidget_Menu::SwitchWidget(int32 Index, bool IsForced)
{
	if (MenuWS->GetActiveWidgetIndex() == Index&&
		!IsForced)
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
		SkipBtn->SetVisibility(ESlateVisibility::Visible);
		AutoBtn->SetVisibility(ESlateVisibility::Visible);
		ReplayBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BGImg->SetVisibility(ESlateVisibility::Visible);
		SkipBtn->SetVisibility(ESlateVisibility::Collapsed);
		AutoBtn->SetVisibility(ESlateVisibility::Collapsed);
		ReplayBtn->SetVisibility(ESlateVisibility::Collapsed);
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
	mDialogueWidget->AddToViewport(0);
	Option->Init(mDialogueWidget, PersistantData);
	Option->InitializeSavedOptions();
	Codex->CreateCodexButtons(mDialogueWidget);
	SaveLoad->CreateSaveButtons(this);
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

void UWidget_Menu::Save(FString SlotName, bool WillGameExit)
{
	USG_VN* saveData = Cast<USG_VN>(
		UGameplayStatics::CreateSaveGameObject(USG_VN::StaticClass()));
	IInterface_VNSave::Execute_OnSaveGame(GetGameInstance(), saveData);
	IInterface_VNSave::Execute_OnSaveGame(History, saveData);
	UGameplayStatics::SaveGameToSlot(saveData, SlotName, 0);
	if (IsInGame()&&
		!WillGameExit)
	{
		ToggleForScreenshot();
		UBFL_VN::TakeScreenshotOfUI(SlotName);
		SaveLoad->SetScreenshotIndex(SlotName);
	}
}

void UWidget_Menu::Load(FString SlotName)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return;
	}
	ChangeScene(0);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this,
		&ThisClass::OnBackBtnClicked, FadeBlackAnim->GetEndTime() * 0.5f);
	USG_VN* saveData = Cast<USG_VN>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	IInterface_VNSave::Execute_OnLoadGame(GetGameInstance(), saveData);
	IInterface_VNSave::Execute_OnLoadGame(History, saveData);
	Codex->UpdateAllCodex();
}

void UWidget_Menu::ToggleForScreenshot(bool TurnAllWidget)
{
	if(TurnAllWidget)
	{
		mDialogueWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BGImg->SetRenderOpacity(1.f);
		MenuWS->SetRenderOpacity(1.f);
	}
	else if(BGImg->GetRenderOpacity()==1.f)
	{
		mDialogueWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BGImg->SetRenderOpacity(0.f);
		MenuWS->SetRenderOpacity(0.f);
	}
	else
	{
		mDialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
		BGImg->SetRenderOpacity(1.f);
		MenuWS->SetRenderOpacity(1.f);
	}
}

void UWidget_Menu::PlayCredit(bool CanSkip)
{
	ChangeScene(6);
	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this,
		&ThisClass::Credit, FadeBlackAnim->GetEndTime() * 0.5f);
}

#include "UI/Widget_Menu.h"
#include "UI/Widget_Dialogue.h"
#include "UI/Widget_History.h"
#include "UI/Widget_Codex.h"
#include "BFL/BFL_VN.h"
#include "../VisualNovel.h"
#include "DlgSystem/DlgContext.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include <Kismet/KismetSystemLibrary.h>

UWidget_Menu::UWidget_Menu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UWidget_Menu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	StartBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartBtnClicked);
	HistoryBtn->OnClicked.AddDynamic(this, &ThisClass::OnHistoryBtnClicked);
	CodexBtn->OnClicked.AddDynamic(this, &ThisClass::OnCodexBtnClicked);
	SkipBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	AutoBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	QuickSaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	QuickLoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
	MenuBtn->OnClicked.AddDynamic(this, &ThisClass::OnMenuBtnClicked);
	OptionBtn->OnClicked.AddDynamic(this, &ThisClass::OnOptionBtnClicked);
	GalleryBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackBtnClicked);
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
	mDialogueWidget->StartDialogue(UBFL_VN::mDialogue);
	mDialogueWidget->AddToViewport(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnHistoryBtnClicked()
{
	if (MenuWS->GetActiveWidgetIndex() == 1)
	{
		OnBackBtnClicked();
		return;
	}
	if (IsValid(mDialogueWidget))
	{
		mDialogueWidget->RemoveFromParent();
	}
	MenuWS->SetActiveWidgetIndex(1);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnCodexBtnClicked()
{
	if (MenuWS->GetActiveWidgetIndex() == 3)
	{
		OnBackBtnClicked();
		return;
	}
	if (IsValid(mDialogueWidget))
	{
		mDialogueWidget->RemoveFromParent();
	}
	Codex->UpdateRecentCodexDetail();
	MenuWS->SetActiveWidgetIndex(3);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnMenuBtnClicked()
{
	MenuWS->SetActiveWidgetIndex(0);
	mDialogueWidget->RemoveFromParent();
	mDialogueWidget->StartDialogue(nullptr);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnOptionBtnClicked()
{
	if(MenuWS->GetActiveWidgetIndex()==2)
	{
		OnBackBtnClicked();
		return;
	}
	MenuWS->SetActiveWidgetIndex(2);
	if(IsValid(mDialogueWidget))
	{
		mDialogueWidget->RemoveFromParent();
	}
	UpdateButtonVisibility();
}

void UWidget_Menu::OnBackBtnClicked()
{
	if (IsInGame())
	{
		mDialogueWidget->AddToViewport(0);
	}
	MenuWS->SetActiveWidgetIndex(0);
	UpdateButtonVisibility();
}

void UWidget_Menu::OnQuitBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(),EQuitPreference::Quit,false);
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
		QuickSaveBtn->SetVisibility(ESlateVisibility::Visible);
		QuickLoadBtn->SetVisibility(ESlateVisibility::Visible);
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
		QuickSaveBtn->SetVisibility(ESlateVisibility::Collapsed);
		QuickLoadBtn->SetVisibility(ESlateVisibility::Collapsed);
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

bool UWidget_Menu::IsInGame()
{
	if(!IsValid(mDialogueWidget))
	{
		return false;
	}
	return IsValid(mDialogueWidget->GetDialogueContext());
}

void UWidget_Menu::Init(UWidget_Dialogue* DialogueWidget)
{
	mDialogueWidget = DialogueWidget;
	mDialogueWidget->Init(this);
	Codex->CreateCodexButtons(mDialogueWidget);
}

void UWidget_Menu::AddEntry(FText Name, FText EntryText)
{
	History->AddEntry(Name, EntryText);
}

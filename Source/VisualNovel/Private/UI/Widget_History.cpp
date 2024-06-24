#include "UI/Widget_History.h"
#include "UI/Widget_HistoryEntry.h"
#include "Save/SG_VN.h"
#include "../VisualNovel.h"
#include "Components/ScrollBox.h"
#include <Components/ScrollBoxSlot.h>

UWidget_History::UWidget_History(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mHistoryEntryClass, UUserWidget, "/Game/VN/UI/WP_HistoryEntry.WP_HistoryEntry_C");

	mMaxHistoryLength = 150;
}

void UWidget_History::OnNewGame_Implementation()
{
	HistorySB->ClearChildren();
}

void UWidget_History::OnSaveGame_Implementation(USG_VN* SaveGame)
{
	SaveGame->mHistory.Empty();
	for (int32 i = 0; i < HistorySB->GetChildrenCount();++i)
	{
		UWidget_HistoryEntry*  historyEntry = 
			Cast<UWidget_HistoryEntry>(HistorySB->GetChildAt(i));
		SaveGame->mHistory.Add(historyEntry->GetEntryString());
	}
}

void UWidget_History::OnLoadGame_Implementation(USG_VN* SaveGame)
{
	HistorySB->ClearChildren();
	for (int32 i = 0; i < SaveGame->mHistory.Num(); ++i)
	{
		FString leftStr;
		FString rightStr;
		SaveGame->mHistory[i].Split(TEXT("|"), &leftStr, &rightStr);
		AddEntry(FText::FromString(leftStr), FText::FromString(rightStr));
	}
}

void UWidget_History::AddEntry(FText Name, FText EntryText)
{
	if(mLatestNameText.EqualTo(Name)&&
		mLatestEntryText.EqualTo(EntryText))
	{
		return;
	}
	if (!IsValid(mHistoryEntryClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_History::mHistoryEntryClass 클래스 없음"));
		return;
	}
	UWidget_HistoryEntry* historyEntry = nullptr;
	if (HistorySB->GetChildrenCount() >= mMaxHistoryLength)
	{
		historyEntry = Cast<UWidget_HistoryEntry>(HistorySB->GetChildAt(0));
	}
	else
	{
		historyEntry = CreateWidget<UWidget_HistoryEntry>(GetOwningPlayer(), mHistoryEntryClass);
	}
	if (!IsValid(historyEntry))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_History::historyEntry 캐스팅 실패"));
		return;
	}
	mLatestNameText = Name;
	mLatestEntryText = EntryText;
	historyEntry->UpdateEntry(Name, EntryText);
	HistorySB->AddChild(historyEntry);
	HistorySB->ScrollToEnd();
	UScrollBoxSlot* slot = Cast<UScrollBoxSlot>(historyEntry->Slot);
	if (IsValid(slot))
	{
		slot->SetPadding(FMargin(0.f, 0.f, 0.f, 35.f));
	}
}
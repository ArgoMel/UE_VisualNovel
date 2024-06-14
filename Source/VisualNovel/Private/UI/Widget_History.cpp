#include "UI/Widget_History.h"
#include "UI/Widget_HistoryEntry.h"
#include "../VisualNovel.h"
#include "Components/ScrollBox.h"

UWidget_History::UWidget_History(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetClassAsset(mHistoryEntryClass, UUserWidget, "/Game/VN/UI/WP_HistoryEntry.WP_HistoryEntry_C");

	mMaxHistoryLength = 5;
}

void UWidget_History::AddEntry(FText Name, FText EntryText)
{
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
	historyEntry->UpdateEntry(Name, EntryText);
	HistorySB->AddChild(historyEntry);
	HistorySB->ScrollToEnd();
}
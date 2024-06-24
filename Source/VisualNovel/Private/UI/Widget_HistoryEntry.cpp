#include "UI/Widget_HistoryEntry.h"
#include "Components/RichTextBlock.h"

UWidget_HistoryEntry::UWidget_HistoryEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWidget_HistoryEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWidget_HistoryEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_HistoryEntry::UpdateEntry(FText Name, FText EntryText)
{
	SpeakerName->SetText(Name);
	Entry->SetText(EntryText);
}

FString UWidget_HistoryEntry::GetEntryString()
{
	return SpeakerName->GetText().ToString()+TEXT("|")+ Entry->GetText().ToString();
}

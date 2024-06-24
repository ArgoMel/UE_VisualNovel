#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HistoryEntry.generated.h"

class URichTextBlock;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_HistoryEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_HistoryEntry(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> SpeakerName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> Entry;

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void UpdateEntry(FText Name,FText EntryText);

	FString GetEntryString();
};

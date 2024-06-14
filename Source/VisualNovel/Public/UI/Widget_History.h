#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_History.generated.h"

class UScrollBox;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_History : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_History(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScrollBox> HistorySB;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mHistoryEntryClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	int32 mMaxHistoryLength;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void AddEntry(FText Name, FText EntryText);
};

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interface_VNSave.h"
#include "Widget_History.generated.h"

class UScrollBox;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_History : public UUserWidget
	, public IInterface_VNSave
{
	GENERATED_BODY()
public:
	UWidget_History(const FObjectInitializer& ObjectInitializer);
public:
	void OnNewGame_Implementation();

private:
	FText mLatestNameText;
	FText mLatestEntryText;

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

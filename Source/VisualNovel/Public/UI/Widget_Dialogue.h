#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Dialogue.generated.h"

class UDlgContext;

UCLASS()
class VISUALNOVEL_API UWidget_Dialogue : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Dialogue(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<UDlgContext> mDIalogueContext;
};

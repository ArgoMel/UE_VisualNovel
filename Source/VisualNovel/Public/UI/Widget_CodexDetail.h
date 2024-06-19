#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_CodexDetail.generated.h"

class URichTextBlock;

UCLASS()
class VISUALNOVEL_API UWidget_CodexDetail : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> CodexText;

public:
	void Init(FText Text);
};

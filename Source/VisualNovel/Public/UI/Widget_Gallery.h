#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Gallery.generated.h"

class UUniformGridPanel;
class UPersistantData;

UCLASS(Abstract)
class VISUALNOVEL_API UWidget_Gallery : public UUserWidget
{
	GENERATED_BODY()
public:
	UWidget_Gallery(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GalleryUGP;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mGalleryBtnClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> mGalleryImgClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Variable")
	int32 mImagesPerRow;

public:
	UFUNCTION(BlueprintCallable, Category = "Event")
	void CreateGalleryMenu(UPersistantData* PersistantData);

	void UpdateGallery(FString TextureName);
};

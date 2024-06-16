#pragma once
#include "CoreMinimal.h"
#include "VNStruct.generated.h"

UENUM(BlueprintType)
enum class EParticipantName : uint8
{
	Yuri,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FParticipantData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> CharacterTexture;
};
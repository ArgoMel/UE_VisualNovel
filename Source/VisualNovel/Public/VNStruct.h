#pragma once
#include "CoreMinimal.h"
#include "VNStruct.generated.h"

UENUM(BlueprintType)
enum class EParticipantName : uint8
{
	Yuri,
	Eina,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBGImgData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> Texture;

	FBGImgData()
	{
	}
};

USTRUCT(BlueprintType)
struct FParticipantData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName,UTexture2D*> Expressions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> Texture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Alignment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;

	FParticipantData()
		: Alignment(0.5, 0.65)
		, Scale(0.6f)
	{
	}
};
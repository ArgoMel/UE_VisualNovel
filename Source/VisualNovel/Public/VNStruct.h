#pragma once
#include "../VisualNovel.h"
#include "VNStruct.generated.h"

UENUM(BlueprintType)
enum class ESoundKind : uint8
{
	Master,
	BGM,
	Voice,
	SFX,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EParticipantName : uint8
{
	Yuri,
	Eina,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FCharacterSaveInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Expression;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Position;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EParticipantName ParticipantName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Opacity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIntroduced;

	FCharacterSaveInfo()
	{
		Expression = VN_START_EXPRESSION;
		Position = FVector2D::ZeroVector;
		ParticipantName = EParticipantName::Yuri;
		Opacity = 1.f;
		bIntroduced = false;
	}
};

USTRUCT(BlueprintType)
struct FStringArray
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> Strings;

	FStringArray()
	{
		
	}
	FStringArray(FString String)
	{
		Strings.Add(String);
	}
	FStringArray(TArray<FString>& Array)
	{
		Strings = Array;
	}
};

USTRUCT(BlueprintType)
struct FParticipantData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName,UTexture2D*> Expressions;	//말하는 캐릭터 이미지의 개수
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Alignment;					//초기 위치
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;							//초기 크기

	FParticipantData()
		: Alignment(0.5, 0.65)
		, Scale(0.6f)
	{
	}
};
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DlgSystem/DlgDialogueParticipant.h>
#include "Participant.generated.h"

UCLASS()
class VISUALNOVEL_API AParticipant : public AActor
	, public IDlgDialogueParticipant
{
	GENERATED_BODY()
public:	
	AParticipant();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	FName GetParticipantName_Implementation() const;
	FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const;
	bool ModifyIntValue_Implementation(FName ValueName, bool bDelta, int32 Value);

public:
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FText mPlayerName;
	UPROPERTY(BlueprintReadWrite, Category = "Variable")
	FText mPlayerNameEnding;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Variable")
	int32 mAffection;
};

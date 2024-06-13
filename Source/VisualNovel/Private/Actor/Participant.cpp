#include "Actor/Participant.h"
#include "../VisualNovel.h"

AParticipant::AParticipant()
{
	PrimaryActorTick.bCanEverTick = false;

	mPlayerNameEnding = FText::FromString(TEXT("san"));
	mAffection = 0;
}

void AParticipant::BeginPlay()
{
	Super::BeginPlay();
}

void AParticipant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AParticipant::GetParticipantName_Implementation() const
{
	return ISPHERIA;
}

FText AParticipant::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return FText::FromString(ISPHERIA);
}

bool AParticipant::ModifyIntValue_Implementation(FName ValueName, bool bDelta, int32 Value)
{
	if(ValueName==AFFECTION)
	{
		if (bDelta)
		{
			mAffection += Value;
		}
		else
		{
			mAffection = Value;
		}

		if(mAffection>=3)
		{
			mPlayerNameEnding = FText::FromString(TEXT("kun"));
		}
		else
		{
			mPlayerNameEnding = FText::FromString(TEXT("san"));
		}
	}
	return false;
}


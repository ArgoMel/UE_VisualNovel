#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_VN.generated.h"

UCLASS()
class VISUALNOVEL_API AGM_VN : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGM_VN();
protected:
	virtual void BeginPlay() override;
};

// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "CSCrosshairWidget.generated.h"

class ACSCharacter;

/**
 * 
 */
UCLASS()
class COOPSHOOTER_API UCSCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	void SetOwnerCharacter(ACSCharacter* OwnerCharacter);

protected:
	UPROPERTY(BlueprintReadOnly)
	ACSCharacter* Owner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxCrosshairDistance;
};

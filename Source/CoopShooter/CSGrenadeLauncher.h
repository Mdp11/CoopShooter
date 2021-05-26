// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CSBaseWeapon.h"

#include "CSGrenadeLauncher.generated.h"

class ACSGrenade;

UCLASS()
class COOPSHOOTER_API ACSGrenadeLauncher : public ACSBaseWeapon
{
	GENERATED_BODY()

public:
	ACSGrenadeLauncher();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ACSGrenade> GrenadeClass;

	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate;

	bool bCanFire;

	void ResetCanFire();

public:
	virtual void Fire() override;
};

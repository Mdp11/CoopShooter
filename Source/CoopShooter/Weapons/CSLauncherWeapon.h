// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponBase.h"

#include "CSLauncherWeapon.generated.h"

class ACSProjectile;

UCLASS()
class COOPSHOOTER_API ACSLauncherWeapon : public ACSWeaponBase
{
	GENERATED_BODY()

public:
	ACSLauncherWeapon();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ACSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	bool bOverrideProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(EditCondition=bOverrideProjectileMovement))
	float InitialVelocity;

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(EditCondition=bOverrideProjectileMovement))
	float MaxVelocity;

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(EditCondition=bOverrideProjectileMovement))
	bool bShouldBounce;

	void SpawnProjectile(const FRotator& PlayerViewPointRotation, const FVector& MuzzleLocation) const;

	void SpawnProjectileDefaultMovement(const FRotator& PlayerViewPointRotation, const FVector& MuzzleLocation) const;

	void SpawnProjectileOverriddenMovement(const FRotator& PlayerViewPointRotation, const FVector& MuzzleLocation) const;

public:
	virtual void Fire() override;
};

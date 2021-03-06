// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CSProjectile.h"
#include "CSGrenadeProjectile.generated.h"

UCLASS()
class COOPSHOOTER_API ACSGrenadeProjectile : public ACSProjectile
{
	GENERATED_BODY()

public:
	ACSGrenadeProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category="Explosion")
	float SecondsToExplode;

	virtual void Explode();

	virtual void BeginPlay() override;
};

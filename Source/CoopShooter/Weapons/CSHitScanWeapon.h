// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CSWeaponBase.h"
#include "CSHitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOPSHOOTER_API ACSHitScanWeapon : public ACSWeaponBase
{
	GENERATED_BODY()

public:
	ACSHitScanWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* TracerEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="FX")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, Category="Fire")
	float HeadShotMultiplier;

	UPROPERTY(EditDefaultsOnly, Category="Fire")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category="Fire")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire")
	TSubclassOf<UDamageType> DamageType;

	virtual void PlayTracerEffects(const FVector Location) const;

	void PlayImpactEffects(const FHitResult& HitResult);

	virtual void Fire() override;
};

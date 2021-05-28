// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CSBaseWeapon.h"
#include "CSRifle.generated.h"

/**
* 
*/
UCLASS()
class COOPSHOOTER_API ACSRifle : public ACSBaseWeapon
{
	GENERATED_BODY()

public:
	ACSRifle();

protected:

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* DefaultImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* FleshImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* TracerEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="FX")
	FName TracerTargetName;

	virtual void PlayTracerEffects(const FVector Location) const;

	void PlayImpactEffects(const FHitResult& HitResult);

	virtual void Fire() override;
};

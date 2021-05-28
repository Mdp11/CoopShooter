// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSBaseWeapon.generated.h"

UCLASS(Abstract, Blueprintable)
class COOPSHOOTER_API ACSBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACSBaseWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float BaseDamage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate;

	bool bCanFire;

	void ResetCanFire();

	virtual void PlayFireEffects() const;

	virtual void Fire() PURE_VIRTUAL(ACSBaseWeapon::Fire, );

	virtual void HandleCanFireReset();


public:
	virtual void StartFire();
};

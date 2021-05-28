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

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="FX")
	TSubclassOf<UCameraShakeBase> FireCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category="Base Stats")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category="Base Stats")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Base Stats")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Base Stats")
	float FireRate;
	
	FTimerHandle TimerHandle_HandleFire;

	float LastFireTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleSocketName;


	UPROPERTY(EditDefaultsOnly, Category="Recoil")
	FRotator MaxRecoil;

	UPROPERTY(EditDefaultsOnly, Category="Recoil")
	FRotator MinRecoil;

	float OverheatLevel;

	UPROPERTY(EditDefaultsOnly, Category="Overheat")
	bool bCanOverheat;
	
	UPROPERTY(EditDefaultsOnly, Category="Overheat",  meta=(EditCondition="bCanOverheat"))
	float OverheatIncreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float OverheatDecreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float OverheatRecoverRate;

	FTimerHandle TimerHandle_OverheatRecover;

	virtual void PlayFireEffects() const;

	virtual void Fire() PURE_VIRTUAL(ACSBaseWeapon::Fire, );

	void AddRecoil() const;

	void HandleOverheat();
	
	virtual void HandleFire();

	virtual void OverheatRecover();


public:
	
	virtual void StartFire();
	
	virtual void StopFire();
};

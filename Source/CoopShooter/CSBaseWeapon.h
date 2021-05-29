// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSBaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Idle,
	Firing,
	Reloading
};

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

	UPROPERTY(EditDefaultsOnly, Category="Overheat")
	bool bCanOverheat;

	float CurrentOverheat;

	float MinOverheat;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float MaxOverheat;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float OverheatIncreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float OverheatDecreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Overheat", meta=(EditCondition="bCanOverheat"))
	float OverheatRecoverRate;

	FTimerHandle TimerHandle_OverheatRecover;

	UPROPERTY(EditDefaultsOnly, Category="Spread")
	bool bHasSpread;

	UPROPERTY(BlueprintReadOnly)
	;
	float CurrentSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spread", meta=(EditCondition="bHasSpread"))
	float MinSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spread", meta=(EditCondition="bHasSpread"))
	float MaxSpread;

	UPROPERTY(EditDefaultsOnly, Category="Spread", meta=(EditCondition="bHasSpread"))
	float SpreadIncreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Spread", meta=(EditCondition="bHasSpread"))
	float SpreadDecreaseSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Spread", meta=(EditCondition="bHasSpread"))
	float SpreadRecoverRate;

	FTimerHandle TimerHandle_SpreadRecover;

	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=0))
	int CurrentMagazineAmmo;

	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=1))
	int MaxMagazineAmmo;

	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=0))
	int CurrentBackupAmmo;

	UPROPERTY(EditDefaultsOnly, Category="Ammo", meta=(ClampMin=0))
	int MaxBackupAmmo;

	FTimerHandle TimerHandle_Reload;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadAnim;

	EWeaponState State;

	virtual void PlayFireEffects() const;

	virtual void Fire() PURE_VIRTUAL(ACSBaseWeapon::Fire,);

	void AddRecoil() const;

	void HandleOverheat();

	void HandleSpread();

	void StartReloading();

	void StopReloading();

	void Reload();
	
	virtual void HandleFire();

	virtual void OverheatRecover();

	void SpreadRecover();

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName) const;

	void SetWeaponState(const EWeaponState& WeaponState);

	void RecoverWeapon();

public:
	virtual void RequestStartFire();

	virtual void RequestStopFire();

	virtual void RequestReload();

	virtual void UnEquip();

	bool IsFiring() const;

	int GetCurrentMagazineAmmo() const;

	int GetCurrentBackupAmmo() const;
};

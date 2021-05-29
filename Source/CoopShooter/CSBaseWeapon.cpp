// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

#include "CoopShooter.h"
#include "CSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACSBaseWeapon::ACSBaseWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	Range = 10000.f;
	BaseDamage = 20.f;
	FireRate = 0.1f;

	MaxRecoil = FRotator{0.15, 0.1, 0.0};
	MinRecoil = FRotator{0.1, -0.1, 0.0};

	MuzzleSocketName = "MuzzleFlashSocket";

	bCanOverheat = true;
	
	MinOverheat = 1.f;
	MaxOverheat = 10.f;
	CurrentOverheat = MinOverheat;

	CurrentOverheat = 1.f;
	OverheatIncreaseSpeed = 1.1f;
	OverheatDecreaseSpeed = 0.7f;
	OverheatRecoverRate = 0.015f;

	bHasSpread = true;

	MinSpread = 1.f;
	MaxSpread = 5.f;
	CurrentSpread = MinSpread;
	
	SpreadIncreaseSpeed = 1.1f;
	SpreadDecreaseSpeed = 0.9f;
	SpreadRecoverRate = 0.08f;

	NoAnimReloadDuration = 1.f;

	State = EWeaponState::Idle;
}

void ACSBaseWeapon::PlayFireEffects() const
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
		if (PC)
		{
			PC->ClientStartCameraShake(FireCameraShake);
		}
	}
}

void ACSBaseWeapon::AddRecoil() const
{
	ACSCharacter* OwnerCharacter = Cast<ACSCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		FRotator Recoil = {
			FMath::RandRange(MinRecoil.Pitch, MaxRecoil.Pitch), FMath::RandRange(MinRecoil.Yaw, MaxRecoil.Yaw), 0.f
		};

		Recoil.Pitch *= CurrentOverheat;
		Recoil.Yaw *= FMath::Min(CurrentOverheat * 0.3f, MinOverheat);
		OwnerCharacter->AddRecoil(Recoil);
	}
}

void ACSBaseWeapon::HandleOverheat()
{
	if (bCanOverheat)
	{
		CurrentOverheat = FMath::Min(CurrentOverheat * OverheatIncreaseSpeed, MaxOverheat);
	}
}

void ACSBaseWeapon::HandleSpread()
{
	if (bHasSpread)
	{
		CurrentSpread = FMath::Min(CurrentSpread * SpreadIncreaseSpeed, MaxSpread);
	}
}

void ACSBaseWeapon::StartReloading()
{
	if(CurrentBackupAmmo > 0 && CurrentMagazineAmmo < MaxMagazineAmmo && State != EWeaponState::Reloading)
	{
		State = EWeaponState::Reloading;

		float AnimDuration = PlayWeaponAnimation(ReloadAnim);
		if (AnimDuration <= 0.0f)
		{
			AnimDuration = NoAnimReloadDuration;
		}
		
		// GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &ACS::StopSimulateReload, AnimDuration, false);
		
		GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &ACSBaseWeapon::Reload, FMath::Max(0.1f, AnimDuration - 0.1f), false);
		
		// GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &ACSBaseWeapon::Reload, 1.f);
	}
}

void ACSBaseWeapon::StopReloading()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Reload);
}

void ACSBaseWeapon::Reload()
{
	StopWeaponAnimation(ReloadAnim);
	const int NMissingAmmo = MaxMagazineAmmo - CurrentMagazineAmmo;
	const int NActualReloadedAmmo = FMath::Min(CurrentBackupAmmo, NMissingAmmo);
	CurrentMagazineAmmo += NActualReloadedAmmo;
	CurrentBackupAmmo -= NActualReloadedAmmo;
}

void ACSBaseWeapon::HandleFire()
{
	if(CurrentMagazineAmmo > 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
		GetWorldTimerManager().ClearTimer(TimerHandle_SpreadRecover);
		
		SetWeaponState(EWeaponState::Firing);
		PlayFireEffects();
		Fire();
		HandleOverheat();
		HandleSpread();
		AddRecoil();
		LastFireTime = GetWorld()->GetTimeSeconds();
		CurrentMagazineAmmo--;
	}
	else
	{
		RecoverWeapon();
		StartReloading();
	}
}

void ACSBaseWeapon::OverheatRecover()
{
	CurrentOverheat = FMath::Max(CurrentOverheat * OverheatDecreaseSpeed, MinOverheat);
	if (FMath::IsNearlyEqual(CurrentOverheat, MinOverheat))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
	}
}

void ACSBaseWeapon::SpreadRecover()
{
	CurrentSpread = FMath::Max(CurrentSpread * SpreadDecreaseSpeed, MinSpread);
	if (FMath::IsNearlyEqual(CurrentSpread, MinSpread))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_SpreadRecover);
	}
}

float ACSBaseWeapon::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName) const
{
	float Duration = 0.0f;
	
	ACSCharacter* OwnerPlayer = Cast<ACSCharacter>(GetOwner());
		
	if (OwnerPlayer)
	{
		if (Animation)
		{
			Duration = OwnerPlayer->PlayAnimMontage(Animation, InPlayRate, StartSectionName);
		}
	}
	return Duration;
}

void ACSBaseWeapon::StopWeaponAnimation(UAnimMontage* Animation) const
{
	ACSCharacter* OwnerPlayer = Cast<ACSCharacter>(GetOwner());
		
	if (OwnerPlayer)
	{
		if (Animation)
		{
			OwnerPlayer->StopAnimMontage(Animation);
		}
	}
}

void ACSBaseWeapon::SetWeaponState(const EWeaponState& WeaponState)
{
	State = WeaponState;
}

void ACSBaseWeapon::RecoverWeapon()
{
	if (bCanOverheat)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_OverheatRecover, this, &ACSBaseWeapon::OverheatRecover,
										OverheatRecoverRate, true);
	}
	
	if (bHasSpread)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpreadRecover, this, &ACSBaseWeapon::SpreadRecover,
										SpreadRecoverRate, true);
	}
}

void ACSBaseWeapon::RequestStartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &ACSBaseWeapon::HandleFire, FireRate, true,
	                                FirstDelay);
}

void ACSBaseWeapon::RequestStopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
	RecoverWeapon();

	if(State != EWeaponState::Reloading)
	{
		SetWeaponState(EWeaponState::Idle);
	}
}

void ACSBaseWeapon::RequestReload()
{
	StartReloading();
}

void ACSBaseWeapon::UnEquip()
{
	RequestStopFire();
	StopReloading();
	SetWeaponState(EWeaponState::Idle);
}

bool ACSBaseWeapon::IsFiring() const
{
	return State == EWeaponState::Firing;
}

int ACSBaseWeapon::GetCurrentMagazineAmmo() const
{
	return CurrentMagazineAmmo;
}

int ACSBaseWeapon::GetCurrentBackupAmmo() const
{
	return CurrentBackupAmmo;
}

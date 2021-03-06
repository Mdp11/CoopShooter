// © 2021 Mattia De Prisco All Rights Reserved


#include "CSWeaponBase.h"

#include "CoopShooter/CoopShooter.h"
#include "CoopShooter/Characters/CSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACSWeaponBase::ACSWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

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
	CurrentSpread = 0.f;
	BaseSpread = MinSpread;

	SpreadIncreaseSpeed = 1.1f;
	SpreadDecreaseSpeed = 0.9f;
	SpreadRecoverRate = 0.08f;

	State = EWeaponState::Idle;

	bReplicates = true;
}

void ACSWeaponBase::PlayFireEffects() const
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

void ACSWeaponBase::ServerFire_Implementation()
{
	PlayFireEffects();
	Fire();
}

bool ACSWeaponBase::ServerFire_Validate()
{
	return true;
}

void ACSWeaponBase::AddRecoil() const
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

void ACSWeaponBase::HandleOverheat()
{
	if (bCanOverheat)
	{
		CurrentOverheat = FMath::Min(CurrentOverheat * OverheatIncreaseSpeed, MaxOverheat);
	}
}

void ACSWeaponBase::HandleSpread()
{
	if (bHasSpread)
	{
		CurrentSpread = FMath::Min(CurrentSpread * SpreadIncreaseSpeed, MaxSpread);
	}
}

void ACSWeaponBase::StartReloading()
{
	if (CurrentBackupAmmo > 0 && CurrentMagazineAmmo < MaxMagazineAmmo && State != EWeaponState::Reloading)
	{
		State = EWeaponState::Reloading;

		ACSCharacter* OwnerCharacter = Cast<ACSCharacter>(GetOwner());

		if (OwnerCharacter)
		{
			const float AnimDuration = OwnerCharacter->PlayWeaponAnimation(ReloadAnim);

			GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &ACSWeaponBase::Reload,
			                                FMath::Max(0.1f, AnimDuration - 0.1f), false);
		}
	}
}

void ACSWeaponBase::StopReloading()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Reload);
}

void ACSWeaponBase::Reload()
{
	StopWeaponAnimation(ReloadAnim);
	const int NMissingAmmo = MaxMagazineAmmo - CurrentMagazineAmmo;
	const int NActualReloadedAmmo = FMath::Min(CurrentBackupAmmo, NMissingAmmo);
	CurrentMagazineAmmo += NActualReloadedAmmo;
	CurrentBackupAmmo -= NActualReloadedAmmo;
	SetWeaponState(EWeaponState::Idle);
}

void ACSWeaponBase::HandleFire()
{
	if (CurrentMagazineAmmo > 0 && State != EWeaponState::Reloading)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
		GetWorldTimerManager().ClearTimer(TimerHandle_SpreadRecover);

		SetWeaponState(EWeaponState::Firing);
		CurrentMagazineAmmo--;
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerFire();
		}

		PlayFireEffects();
		Fire();

		HandleOverheat();
		HandleSpread();
		AddRecoil();
		LastFireTime = GetWorld()->GetTimeSeconds();
	}

	if (CurrentMagazineAmmo <= 0)
	{
		RecoverWeapon();
		StartReloading();
	}
}

void ACSWeaponBase::OverheatRecover()
{
	CurrentOverheat = FMath::Max(CurrentOverheat * OverheatDecreaseSpeed, MinOverheat);
	if (FMath::IsNearlyEqual(CurrentOverheat, MinOverheat))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
	}
}

void ACSWeaponBase::SpreadRecover()
{
	CurrentSpread = FMath::Max(CurrentSpread * SpreadDecreaseSpeed, MinSpread);
	if (FMath::IsNearlyEqual(CurrentSpread, MinSpread))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_SpreadRecover);
	}
}

void ACSWeaponBase::StopWeaponAnimation(UAnimMontage* Animation) const
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

void ACSWeaponBase::SetWeaponState(const EWeaponState& WeaponState)
{
	State = WeaponState;
}

void ACSWeaponBase::RecoverWeapon()
{
	if (bCanOverheat)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_OverheatRecover, this, &ACSWeaponBase::OverheatRecover,
		                                OverheatRecoverRate, true);
	}

	if (bHasSpread)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpreadRecover, this, &ACSWeaponBase::SpreadRecover,
		                                SpreadRecoverRate, true);
	}
}

void ACSWeaponBase::RequestStartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &ACSWeaponBase::HandleFire, FireRate, true,
	                                FirstDelay);
}

void ACSWeaponBase::RequestStopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
	RecoverWeapon();

	if (State != EWeaponState::Reloading)
	{
		SetWeaponState(EWeaponState::Idle);
	}
}

void ACSWeaponBase::RequestReload()
{
	StartReloading();
}

void ACSWeaponBase::UnEquip()
{
	RequestStopFire();
	StopReloading();
	StopWeaponAnimation(ReloadAnim);
	SetWeaponState(EWeaponState::Idle);
}

bool ACSWeaponBase::IsFiring() const
{
	return State == EWeaponState::Firing;
}

int ACSWeaponBase::GetCurrentMagazineAmmo() const
{
	return CurrentMagazineAmmo;
}

int ACSWeaponBase::GetCurrentBackupAmmo() const
{
	return CurrentBackupAmmo;
}

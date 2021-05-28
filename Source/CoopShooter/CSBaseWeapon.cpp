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

	OverheatLevel = 1.f;
	OverheatIncreaseSpeed = 1.1f;
	OverheatDecreaseSpeed = 1.1f;
	OverheatRecoverRate = 0.015f;
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
			FMath::RandRange(MinRecoil.Pitch, MaxRecoil.Pitch), FMath::RandRange(MinRecoil.Yaw, MaxRecoil.Yaw),
			FMath::RandRange(MinRecoil.Roll, MaxRecoil.Roll)
		};

		Recoil.Pitch *= OverheatLevel;
		Recoil.Yaw *= FMath::Min(OverheatLevel * 0.3f, 1.f);
		Recoil.Roll *= FMath::Min(OverheatLevel * 0.3f, 1.f);
		OwnerCharacter->AddRecoil(Recoil);
	}
}

void ACSBaseWeapon::HandleOverheat()
{
	if (bCanOverheat)
	{
		OverheatLevel = FMath::Min(OverheatLevel * OverheatIncreaseSpeed, 10.f);
	}
}

void ACSBaseWeapon::HandleFire()
{
	PlayFireEffects();
	Fire();
	HandleOverheat();
	AddRecoil();
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void ACSBaseWeapon::OverheatRecover()
{
	OverheatLevel = FMath::Max(OverheatLevel * OverheatDecreaseSpeed, 1.f);
	if (FMath::IsNearlyEqual(OverheatLevel, 1.f))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
	}
}

void ACSBaseWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &ACSBaseWeapon::HandleFire, FireRate, true,
	                                FirstDelay);

	GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
}

void ACSBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);

	if (bCanOverheat)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_OverheatRecover, this, &ACSBaseWeapon::OverheatRecover,
		                                OverheatRecoverRate, true);
	}
}

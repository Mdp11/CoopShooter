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
	MaxSpread = 10.f;
	CurrentSpread = MinSpread;
	
	SpreadIncreaseSpeed = 1.1f;
	SpreadDecreaseSpeed = 0.7f;
	SpreadRecoverRate = 0.015f;
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

void ACSBaseWeapon::HandleFire()
{
	PlayFireEffects();
	PlayFireEffects();
	Fire();
	HandleOverheat();
	HandleSpread();
	AddRecoil();
	LastFireTime = GetWorld()->GetTimeSeconds();
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
	LOGF("Decreased spread to = %f", CurrentSpread);
}

void ACSBaseWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &ACSBaseWeapon::HandleFire, FireRate, true,
	                                FirstDelay);

	GetWorldTimerManager().ClearTimer(TimerHandle_OverheatRecover);
	GetWorldTimerManager().ClearTimer(TimerHandle_SpreadRecover);
}

void ACSBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);

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

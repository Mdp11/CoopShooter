// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

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
		const FRotator Recoil = {
			FMath::RandRange(MinRecoil.Pitch, MaxRecoil.Pitch), FMath::RandRange(MinRecoil.Yaw, MaxRecoil.Yaw),
			FMath::RandRange(MinRecoil.Roll, MaxRecoil.Roll)
		};
		OwnerCharacter->AddRecoil(Recoil);
	}
}

void ACSBaseWeapon::HandleFire()
{
	PlayFireEffects();
	Fire();
	AddRecoil();
	LastFireTime = GetWorld()->GetTimeSeconds();
}

void ACSBaseWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &ACSBaseWeapon::HandleFire, FireRate, true,
	                                FirstDelay);
}

void ACSBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
}

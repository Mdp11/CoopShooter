// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACSBaseWeapon::ACSBaseWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	Range = 10000.f;
	BaseDamage = 20.f;
	FireRate = 0.f;
	bCanFire = true;

	MuzzleSocketName = "MuzzleFlashSocket";
}

void ACSBaseWeapon::ResetCanFire()
{
	bCanFire = true;
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

void ACSBaseWeapon::Fire()
{
}	

void ACSBaseWeapon::HandleCanFireReset()
{
	if (FireRate == 0.f)
	{
		ResetCanFire();
	}
	else
	{
		FTimerHandle TimerHandle_ResetCanFire;
		GetWorldTimerManager().SetTimer(TimerHandle_ResetCanFire, this, &ACSBaseWeapon::ResetCanFire, FireRate);
	}
}

void ACSBaseWeapon::StartFire()
{
	if (bCanFire)
	{
		bCanFire = false;
		PlayFireEffects();
		Fire();

		HandleCanFireReset();
	}
}

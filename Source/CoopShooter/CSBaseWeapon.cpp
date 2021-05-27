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

	MuzzleSocketName = "MuzzleFlashSocket";
}

void ACSBaseWeapon::PlayFireEffects() const
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);
}

void ACSBaseWeapon::Fire()
{
}

void ACSBaseWeapon::StartFire()
{
	PlayFireEffects();
	Fire();
}

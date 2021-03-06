// © 2021 Mattia De Prisco All Rights Reserved


#include "CSLauncherWeapon.h"

#include "Kismet/GameplayStatics.h"

#include "Projectiles/CSProjectile.h"

ACSLauncherWeapon::ACSLauncherWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	FireRate = 1.f;

	bCanOverheat = false;
	bHasSpread = false;

	MinRecoil = {0.f, 0.f, 0.f};
	MinRecoil = MaxRecoil;

	bOverrideProjectileMovement = false;

	InitialVelocity = 1000.f;
	MaxVelocity = 1000.f;
	bShouldBounce = true;;
}

void ACSLauncherWeapon::SpawnProjectile(const FRotator& PlayerViewPointRotation, const FVector& MuzzleLocation) const
{
	if (bOverrideProjectileMovement)
	{
		SpawnProjectileOverriddenMovement(PlayerViewPointRotation, MuzzleLocation);
	}
	else
	{
		SpawnProjectileDefaultMovement(PlayerViewPointRotation, MuzzleLocation);
	}
}

void ACSLauncherWeapon::SpawnProjectileDefaultMovement(const FRotator& PlayerViewPointRotation,
                                                       const FVector& MuzzleLocation) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACSProjectile* Projectile = GetWorld()->SpawnActor<ACSProjectile>(
		ProjectileClass, MuzzleLocation, PlayerViewPointRotation,
		SpawnParameters);
	Projectile->SetOwner(GetOwner());
}

void ACSLauncherWeapon::SpawnProjectileOverriddenMovement(const FRotator& PlayerViewPointRotation,
                                                          const FVector& MuzzleLocation) const
{
	FTransform ProjectileTransform;
	ProjectileTransform.SetLocation(MuzzleLocation);
	ProjectileTransform.SetRotation(PlayerViewPointRotation.Quaternion());
	ProjectileTransform.SetScale3D(FVector::OneVector);

	ACSProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACSProjectile>(
		ProjectileClass, ProjectileTransform, GetOwner(), GetOwner()->GetInstigator(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->SetProjectileStats(InitialVelocity, MaxVelocity, bShouldBounce);

	Projectile->FinishSpawning(ProjectileTransform);
}

void ACSLauncherWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner && ProjectileClass)
	{
		SetWeaponState(EWeaponState::Firing);

		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

		SpawnProjectile(PlayerViewPointRotation, MuzzleLocation);
	}
}

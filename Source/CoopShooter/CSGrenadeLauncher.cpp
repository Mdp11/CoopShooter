// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGrenadeLauncher.h"

#include "Kismet/GameplayStatics.h"

#include "CSGrenade.h"

ACSGrenadeLauncher::ACSGrenadeLauncher()
{
	PrimaryActorTick.bCanEverTick = false;

	FireRate = 1.f;

	bCanOverheat = false;
	bHasSpread = false;

	MinRecoil = {0.f, 0.f, 0.f};
	MinRecoil = MaxRecoil;
}

void ACSGrenadeLauncher::Fire()
{
	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner && GrenadeClass)
	{
		SetWeaponState(EWeaponState::Firing);
		
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACSGrenade* Grenade = GetWorld()->SpawnActor<ACSGrenade>(GrenadeClass, MuzzleLocation, PlayerViewPointRotation,
		                                                         SpawnParameters);
		Grenade->SetOwner(GetOwner());
	}

	SetWeaponState(EWeaponState::Idle);;
}

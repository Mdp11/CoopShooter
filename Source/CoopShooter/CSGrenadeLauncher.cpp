// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGrenadeLauncher.h"

#include "Kismet/GameplayStatics.h"

#include "CSGrenade.h"

ACSGrenadeLauncher::ACSGrenadeLauncher()
{
	PrimaryActorTick.bCanEverTick = false;

	FireRate = 1.f;
}

void ACSGrenadeLauncher::Fire()
{
	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner && GrenadeClass)
	{		
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);

		const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACSGrenade* Grenade = GetWorld()->SpawnActor<ACSGrenade>(GrenadeClass, MuzzleLocation, PlayerViewPointRotation,
		                                                         SpawnParameters);
		Grenade->SetOwner(GetOwner());
	}
}

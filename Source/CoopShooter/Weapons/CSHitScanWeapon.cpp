UPROPERTY(BlueprintAssignable, Category="Events")// © 2021 Mattia De Prisco All Rights Reserved


#include "CSHitScanWeapon.h"

#include "CoopShooter/CoopShooter.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ACSHitScanWeapon::ACSHitScanWeapon()
{
	TracerTargetName = "BeamEnd";

	Range = 10000.f;
	BaseDamage = 20.f;

	HeadShotMultiplier = 3.f;
}

void ACSHitScanWeapon::PlayTracerEffects(const FVector Location) const
{
	const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

	UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), TracerEffect, MuzzleLocation);

	if (TracerComponent)
	{
		TracerComponent->SetVectorParameter(TracerTargetName, Location);
	}
}

void ACSHitScanWeapon::PlayImpactEffects(const FHitResult& HitResult)
{
	const EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

	UParticleSystem* ImpactEffect;
	FVector EffectScale{1.f, 1.f, 1.f};

	switch (SurfaceType)
	{
	case SURFACE_FLASH_DEFAULT:
		ImpactEffect = FleshImpactEffect;
		EffectScale = {0.3, 0.3, 0.3};
		break;

	case SURFACE_FLASH_VULNERABLE:
		ImpactEffect = FleshImpactEffect;
		EffectScale = {0.5, 0.5, 0.5};
		break;

	default:
		ImpactEffect = DefaultImpactEffect;
		break;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, FRotator::ZeroRotator,
	                                         EffectScale);
}

void ACSHitScanWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		FVector ShotDirection = PlayerViewPointRotation.Vector();

		float HalfRad = FMath::DegreesToRadians(BaseSpread + CurrentSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad);

		FVector TraceEnd = PlayerViewPointLocation + ShotDirection * Range;

		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(WeaponOwner);
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.bReturnPhysicalMaterial = true;

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewPointLocation, TraceEnd, COLLISION_WEAPON,
		                                         CollisionQueryParams))
		{
			AActor* ActorHit = HitResult.GetActor();


			float ActualDamage = BaseDamage;

			if (UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get()) == SURFACE_FLASH_VULNERABLE)
			{
				ActualDamage *= HeadShotMultiplier;
			}

			UGameplayStatics::ApplyPointDamage(ActorHit, ActualDamage, ShotDirection, HitResult,
			                                   WeaponOwner->GetInstigatorController(), GetOwner(), DamageType);

			PlayImpactEffects(HitResult);

			TracerEndPoint = HitResult.ImpactPoint;
		}

		PlayTracerEffects(TracerEndPoint);
	}
}

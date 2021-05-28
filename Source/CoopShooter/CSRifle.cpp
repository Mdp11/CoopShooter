// © 2021 Mattia De Prisco All Rights Reserved


#include "CSRifle.h"

#include "CoopShooter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ACSRifle::ACSRifle()
{
	TracerTargetName = "BeamEnd";
}

void ACSRifle::PlayTracerEffects(const FVector Location) const
{
	const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

	UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), TracerEffect, MuzzleLocation);

	if (TracerComponent)
	{
		TracerComponent->SetVectorParameter(TracerTargetName, Location);
	}
}

void ACSRifle::PlayImpactEffects(const FHitResult& HitResult)
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
		EffectScale = {0.5, 0.5, 0.5	};
		break;

	default:
		ImpactEffect = DefaultImpactEffect;
		break;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, FRotator::ZeroRotator,
	                                         EffectScale);
}

void ACSRifle::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		FVector ShotDirection = PlayerViewPointRotation.Vector();

		FVector TraceEnd = PlayerViewPointLocation + ShotDirection * Range;

		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(WeaponOwner);
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.bReturnPhysicalMaterial = true;

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewPointLocation, TraceEnd, ECC_Visibility,
		                                         CollisionQueryParams))
		{
			AActor* ActorHit = HitResult.GetActor();

			UGameplayStatics::ApplyPointDamage(ActorHit, BaseDamage, ShotDirection, HitResult,
			                                   WeaponOwner->GetInstigatorController(), this, DamageType);

			PlayImpactEffects(HitResult);

			TracerEndPoint = HitResult.ImpactPoint;
		}

		PlayTracerEffects(TracerEndPoint);
	}
}

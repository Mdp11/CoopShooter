// © 2021 Mattia De Prisco All Rights Reserved


#include "CSRifle.h"

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

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

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewPointLocation, TraceEnd, ECC_Visibility,
												CollisionQueryParams))
		{
			AActor* ActorHit = HitResult.GetActor();

			UGameplayStatics::ApplyPointDamage(ActorHit, BaseDamage, ShotDirection, HitResult,
											WeaponOwner->GetInstigatorController(), this, DamageType);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint,
													HitResult.ImpactNormal.Rotation());

			TracerEndPoint = HitResult.ImpactPoint;
		}
		
		PlayTracerEffects(TracerEndPoint);
	}
}

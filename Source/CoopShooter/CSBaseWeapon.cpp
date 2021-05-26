// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACSBaseWeapon::ACSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	Range = 10000.f;
	BaseDamage = 20.f;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "BeamEnd";
}

void ACSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACSBaseWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);

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

		FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TracerEffect, MuzzleLocation);

		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}

		DrawDebugLine(GetWorld(), PlayerViewPointLocation, TraceEnd, FColor::Red, false, 2, 0, 1.f);
	}
}

void ACSBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

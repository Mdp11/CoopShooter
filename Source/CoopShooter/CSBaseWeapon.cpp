// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ACSBaseWeapon::ACSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);

	Range = 10000.f;
}

void ACSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACSBaseWeapon::Fire() const
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{		
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		
		WeaponOwner->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

		FVector ShotDirection = PlayerViewPointRotation.Vector();

		FVector TraceEnd = PlayerViewPointLocation + ShotDirection * Range;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(WeaponOwner);
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true;
		
		FHitResult HitResult;
		if(GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewPointLocation, TraceEnd, ECC_Visibility, CollisionQueryParams))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.Actor->GetName());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint);
		}
		
		DrawDebugLine(GetWorld(), PlayerViewPointLocation, TraceEnd, FColor::Red, false, 2, 0, 2.f);
	}
}

void ACSBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fire();
}

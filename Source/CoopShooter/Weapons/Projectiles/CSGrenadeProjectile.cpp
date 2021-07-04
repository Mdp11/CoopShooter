// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGrenadeProjectile.h"

#include "Kismet/GameplayStatics.h"

ACSGrenadeProjectile::ACSGrenadeProjectile()
{
	SecondsToExplode = 1.f;
	ExplosionRadius = 300.f;
	Damage = 35.f;
}

void ACSGrenadeProjectile::Explode()
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(),
	                                    ExplosionRadius, DamageType, TArray<AActor*>{},
	                                    GetOwner()->GetOwner(), GetOwner()->GetInstigatorController());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator,
	                                         FVector{
		                                         ExplosionRadius / 60.f, ExplosionRadius / 60.f, ExplosionRadius / 60.f
	                                         });
	Destroy();
}

void ACSGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle_Explosion;
	GetWorldTimerManager().SetTimer(TimerHandle_Explosion, this, &ACSGrenadeProjectile::Explode, SecondsToExplode);
}

// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGrenade.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACSGrenade::ACSGrenade()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetSimulatePhysics(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->bShouldBounce = true;

	SecondsToExplode = 1.f;
	ExplosionRadius = 300.f;
	Damage = 35.f;
}

void ACSGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle_Explosion;
	GetWorldTimerManager().SetTimer(TimerHandle_Explosion, this, &ACSGrenade::Explode, SecondsToExplode);
}

void ACSGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSGrenade::Explode()
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(),
	                                    ExplosionRadius, DamageType, TArray<AActor*>{},
	                                    this, GetOwner()->GetInstigatorController());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator,
	                                         FVector{
		                                         ExplosionRadius / 60.f, ExplosionRadius / 60.f, ExplosionRadius / 60.f
	                                         });
	Destroy();
}

// © 2021 Mattia De Prisco All Rights Reserved


#include "STrackerBot.h"

#include "ChaosInterfaceWrapperCore.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	SetRootComponent(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UCSHealthComponent>("HealthComponent");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	MovementForce = 1000.f;
	RequiredDistanceToTarget = 100.f;
	bUseVelocityChange = false;
	bExploded = false;

	BaseDamage = 100.f;
	DamageRadius = 200.f;
}

FVector ASTrackerBot::GetNextPathPoint() const
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* Path = UNavigationSystemV1::FindPathToActorSynchronously(
		GetWorld(), GetActorLocation(), PlayerCharacter);

	if (Path->PathPoints.Num() > 1)
	{
		return Path->PathPoints[1];
	}

	return GetActorLocation();
}

void ASTrackerBot::HandleTakeDamage(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
                                    const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!MaterialInstanceDynamic)
	{
		MaterialInstanceDynamic = MeshComponent->CreateDynamicMaterialInstance(0, MeshComponent->GetMaterial(0));
	}

	if (MaterialInstanceDynamic)
	{
		MaterialInstanceDynamic->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->GetTimeSeconds());
	}

	if (Health <= 0.f)
	{
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct()
{
	if (!bExploded)
	{
		bExploded = true;
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation());

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, BaseDamage, GetActorLocation(), DamageRadius, nullptr, IgnoredActors,
		                                    this,
		                                    GetInstigatorController(), true);

		Destroy();
	}
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= MovementForce;

		MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}
}

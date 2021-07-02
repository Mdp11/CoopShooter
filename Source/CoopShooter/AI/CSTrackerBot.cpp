// © 2021 Mattia De Prisco All Rights Reserved


#include "CSTrackerBot.h"

#include "ChaosInterfaceWrapperCore.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "CoopShooter/Characters/CSCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ACSTrackerBot::ACSTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	SetRootComponent(MeshComponent);

	HealthComponent = CreateDefaultSubobject<UCSHealthComponent>("HealthComponent");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ACSTrackerBot::HandleTakeDamage);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(200.f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	MovementForce = 1000.f;
	RequiredDistanceToTarget = 100.f;
	bUseVelocityChange = false;
	bExploded = false;

	BaseDamage = 100.f;
	DamageRadius = 200.f;
	SelfDamageInterval = 0.25f;
}

FVector ACSTrackerBot::GetNextPathPoint() const
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* Path = UNavigationSystemV1::FindPathToActorSynchronously(
		GetWorld(), GetActorLocation(), PlayerCharacter);

	if (Path && Path->PathPoints.Num() > 1)
	{
		return Path->PathPoints[1];
	}

	return GetActorLocation();
}

void ACSTrackerBot::HandleTakeDamage(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
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

void ACSTrackerBot::SelfDestruct()
{
	if (!bExploded)
	{
		bExploded = true;
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator,
		                                         FVector{2.f});

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, BaseDamage, GetActorLocation(), DamageRadius, nullptr, IgnoredActors,
		                                    this,
		                                    GetInstigatorController(), true);

		UGameplayStatics::SpawnSoundAttached(ExplosionSound, RootComponent);

		Destroy();
	}
}

void ACSTrackerBot::SelfDamage()
{
	TakeDamage(20.f, FDamageEvent{}, GetInstigatorController(), this);
}

void ACSTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
}

void ACSTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!bSelfDestructing)
	{
		if (ACSCharacter* PlayerPawn = Cast<ACSCharacter>(OtherActor))
		{
			bSelfDestructing = true;
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDestruct, this, &ACSTrackerBot::SelfDamage,
			                                SelfDamageInterval, true,
			                                0.f);

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}
	}
}

void ACSTrackerBot::Tick(float DeltaTime)
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

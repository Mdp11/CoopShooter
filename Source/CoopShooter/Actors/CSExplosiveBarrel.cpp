// © 2021 Mattia De Prisco All Rights Reserved


#include "CSExplosiveBarrel.h"

#include "CoopShooter/CoopShooter.h"
#include "CoopShooter/Components/CSHealthComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACSExplosiveBarrel::ACSExplosiveBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMeshComponent->SetLinearDamping(1.f);
	StaticMeshComponent->SetAngularDamping(1.f);

	HealthComponent = CreateDefaultSubobject<UCSHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ACSExplosiveBarrel::OnHealthChanged);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 250.f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;

	bExploded = false;
	Damage = 150.f;
	ExplosionImpulse = 400.f;
}

void ACSExplosiveBarrel::Explode()
{
	bExploded = true;
	
	RadialForceComponent->FireImpulse();
	
	StaticMeshComponent->AddImpulse(FVector::UpVector * ExplosionImpulse, NAME_None, true);
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, RadialForceComponent->GetComponentLocation(),
	                                    RadialForceComponent->Radius, ExplosionDamageType, TArray<AActor*>{}, this,
	                                    nullptr, false, COLLISION_WEAPON);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	if (ExplodedMaterial)
	{
		StaticMeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}

void ACSExplosiveBarrel::OnHealthChanged(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
                                         const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bExploded && Health <= 0.f)
	{
		Explode();
	}
}

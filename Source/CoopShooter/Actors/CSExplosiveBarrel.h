// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CSExplosiveBarrel.generated.h"

class UCSHealthComponent;
class URadialForceComponent;

UCLASS()
class COOPSHOOTER_API ACSExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	ACSExplosiveBarrel();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UCSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	URadialForceComponent* RadialForceComponent;

	bool bExploded;

	UPROPERTY(EditInstanceOnly, Category="Damage")
	float Damage;

	UPROPERTY(EditInstanceOnly, Category="Impulse")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, Category="Material")
	UMaterial* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UDamageType> ExplosionDamageType;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* ExplosionEffect;

	UFUNCTION()
	void OnHealthChanged(UCSHealthComponent* HealthComp, float Health,
	                     float HealthDelta, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void Explode();
};

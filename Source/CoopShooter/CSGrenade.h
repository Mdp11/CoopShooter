// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CSGrenade.generated.h"

class UProjectileMovementComponent;

UCLASS()
class COOPSHOOTER_API ACSGrenade : public AActor
{
	GENERATED_BODY()

public:
	ACSGrenade();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float SecondsToExplode;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Explode();
};

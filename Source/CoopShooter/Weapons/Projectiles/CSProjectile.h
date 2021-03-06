// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS(Abstract, Blueprintable)
class COOPSHOOTER_API ACSProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACSProjectile();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

public:
	virtual void SetProjectileStats(float InitialSpeed, float MaxSpeed, bool ShouldBounce);
};

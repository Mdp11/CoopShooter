// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "CoopShooter/Components/CSHealthComponent.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class COOPSHOOTER_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UCSHealthComponent* HealthComponent;

	FVector NextPathPoint;

	bool bExploded;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementForce;

	UPROPERTY(EditAnywhere, Category="Movement")
	bool bUseVelocityChange;

	UPROPERTY(EditAnywhere, Category="Movement")
	float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageRadius;

	FVector GetNextPathPoint() const;

	UPROPERTY(EditAnywhere, Category="FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

	UFUNCTION()
	void HandleTakeDamage(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
	                      const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruct();

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

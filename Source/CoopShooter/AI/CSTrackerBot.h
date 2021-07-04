// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "CoopShooter/Components/CSHealthComponent.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"

#include "CSTrackerBot.generated.h"

UCLASS()
class COOPSHOOTER_API ACSTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ACSTrackerBot();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UCSHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USphereComponent* SphereComponent;

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

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float SelfDamageInterval;
	
	FTimerHandle TimerHandle_RefreshPath;

	FVector GetNextPathPoint();

	bool bSelfDestructing;

	UPROPERTY(EditAnywhere, Category="FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue* SelfDestructSound;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundCue* ExplosionSound;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

	FTimerHandle TimerHandle_SelfDestruct;

	UFUNCTION()
	void HandleTakeDamage(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
	                      const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruct();

	void SelfDamage();

	void RefreshPath();

	virtual void BeginPlay() override;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void Tick(float DeltaTime) override;
};

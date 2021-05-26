// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSBaseWeapon.generated.h"

UCLASS()
class COOPSHOOTER_API ACSBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACSBaseWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Statistics")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Statistics")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Statistics")
	TSubclassOf<UDamageType> DamageType;

	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	void Fire();
};

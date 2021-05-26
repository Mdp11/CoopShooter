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

	UPROPERTY(EditDefaultsOnly, Category="WeaponStats")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category="FX")
	UParticleSystem* HitEffect;

	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	void Fire() const;
};

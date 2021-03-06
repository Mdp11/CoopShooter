// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UCSHealthComponent*, HealthComp, float, Health,
                                             float, HealthDelta, const class UDamageType*, DamageType,
                                             class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COOPSHOOTER_API UCSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCSHealthComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Health")
	float CurrentHealth;

	UFUNCTION()
	virtual void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                             class AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSignature OnHealthChanged;

	float GetHealth() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthComponent")
	uint8 Team;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="HealthComponent")
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);
};

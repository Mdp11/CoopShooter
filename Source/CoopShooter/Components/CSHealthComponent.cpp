// © 2021 Mattia De Prisco All Rights Reserved


#include "CSHealthComponent.h"

#include "CoopShooter/CoopShooter.h"

UCSHealthComponent::UCSHealthComponent()
{
	MaxHealth = 100.f;
}


void UCSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	AActor* Owner = GetOwner();
	if(Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UCSHealthComponent::OnTakeAnyDamage);
	}
}

void UCSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	LOGF("Current health = %f", CurrentHealth);
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}


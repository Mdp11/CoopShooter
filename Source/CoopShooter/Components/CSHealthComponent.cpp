// © 2021 Mattia De Prisco All Rights Reserved


#include "CSHealthComponent.h"

UCSHealthComponent::UCSHealthComponent()
{
	MaxHealth = 100.f;
	Team = 255;
}


void UCSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UCSHealthComponent::OnTakeAnyDamage);
	}
}

void UCSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                         class AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageCauser != DamagedActor && IsFriendly(DamageCauser, DamagedActor))
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("%s received %f damage and now has %f health"), *GetOwner()->GetName(), Damage,
	       CurrentHealth);
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

float UCSHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

bool UCSHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (!ActorA || !ActorB)
	{
		return true;
	}

	UCSHealthComponent* HealthCompA = Cast<UCSHealthComponent>(
		ActorA->GetComponentByClass(UCSHealthComponent::StaticClass()));
	UCSHealthComponent* HealthCompB = Cast<UCSHealthComponent>(
		ActorB->GetComponentByClass(UCSHealthComponent::StaticClass()));

	if (!HealthCompA || !HealthCompB)
	{
		return true;
	}

	return HealthCompA->Team == HealthCompB->Team;
}

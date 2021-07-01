// © 2021 Mattia De Prisco All Rights Reserved


#include "STrackerBot.h"

ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	SetRootComponent(MeshComponent);
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
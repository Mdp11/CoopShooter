// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

ACSBaseWeapon::ACSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);
}

void ACSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ACSBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
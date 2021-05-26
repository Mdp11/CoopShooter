// © 2021 Mattia De Prisco All Rights Reserved


#include "CSBaseWeapon.h"

#include "DrawDebugHelpers.h"

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

void ACSBaseWeapon::Fire() const
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		FVector OwnerEyesLocation;
		FRotator OwnerEyesRotation;
		WeaponOwner->GetActorEyesViewPoint(OwnerEyesLocation, OwnerEyesRotation);

		DrawDebugLine(GetWorld(), OwnerEyesLocation, OwnerEyesLocation * 10, FColor::Red, false, 2);
	}
}

void ACSBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

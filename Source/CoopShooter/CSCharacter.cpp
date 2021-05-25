// © 2021 Mattia De Prisco All Rights Reserved


#include "CSCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

ACSCharacter::ACSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACSCharacter::MoveRight);
}

void ACSCharacter::MoveForward(const float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ACSCharacter::MoveRight(const float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

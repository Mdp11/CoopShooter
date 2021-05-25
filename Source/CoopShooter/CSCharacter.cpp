// © 2021 Mattia De Prisco All Rights Reserved


#include "CSCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACSCharacter::ACSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation({0.f, 0.f, 80.f});

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
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
	
	PlayerInputComponent->BindAxis("LookUp", this, &ACSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Pressed, this, &ACSCharacter::Crouch, false);
	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Released, this, &ACSCharacter::UnCrouch, false);
}

void ACSCharacter::MoveForward(const float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ACSCharacter::MoveRight(const float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

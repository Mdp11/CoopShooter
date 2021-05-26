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

	bWalking = false;
	bSprinting = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 900.f;

	WalkVelocityModifier = 0.33f;
	RunVelocityModifier = 0.66f;
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
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACSCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_DoubleClick, this, &ACSCharacter::ActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACSCharacter::DeactivateSprint);
	
	PlayerInputComponent->BindAction("Walk/Run", IE_Pressed, this, &ACSCharacter::SwitchWalkRun);
	PlayerInputComponent->BindAction("Walk/Run", IE_Released, this, &ACSCharacter::SwitchWalkRun);
	PlayerInputComponent->BindAction("Walk/Run Toggle", IE_Pressed, this, &ACSCharacter::SwitchWalkRun);
}

void ACSCharacter::MoveForward(float Value)
{
	Value *= bSprinting ? 1.f : bWalking ? WalkVelocityModifier : RunVelocityModifier;
	AddMovementInput(GetActorForwardVector() * Value);
}

void ACSCharacter::MoveRight(float Value)
{
	
	Value *= bSprinting ? 0.2 : bWalking ? WalkVelocityModifier : RunVelocityModifier;
	AddMovementInput(GetActorRightVector() * Value);
}

void ACSCharacter::SwitchWalkRun()
{
	bWalking = !bWalking;
}

void ACSCharacter::ActivateSprint()
{
	bSprinting = true;
}

void ACSCharacter::DeactivateSprint()
{
	bSprinting = false;
}
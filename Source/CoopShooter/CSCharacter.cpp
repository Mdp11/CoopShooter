// © 2021 Mattia De Prisco All Rights Reserved


#include "CSCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CSBaseWeapon.h"

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
	GetCharacterMovement()->CrouchedHalfHeight = 58.f;

	bWalking = false;
	bSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = 900.f;

	WalkVelocityModifier = 0.33f;
	RunVelocityModifier = 0.66f;
	JumpResetTime = 1.2f;
	bCanJump = true;
}

void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor<ACSBaseWeapon>(WeaponClass, SpawnParameters);

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AAAA"));
	}
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

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACSCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACSCharacter::StopCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACSCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_DoubleClick, this, &ACSCharacter::ActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACSCharacter::DeactivateSprint);

	PlayerInputComponent->BindAction("Walk/Run", IE_Pressed, this, &ACSCharacter::SwitchWalkRun);
	PlayerInputComponent->BindAction("Walk/Run", IE_Released, this, &ACSCharacter::SwitchWalkRun);
	PlayerInputComponent->BindAction("Walk/Run Toggle", IE_Pressed, this, &ACSCharacter::SwitchWalkRun);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACSCharacter::Fire);
}

void ACSCharacter::MoveForward(float Value)
{
	Value *= bSprinting && !bIsCrouched ? 1.f : bWalking ? WalkVelocityModifier : RunVelocityModifier;
	LastMovementInput.X = Value;
	if (!bSprinting && FMath::Abs(LastMovementInput.Y) > 0.f)
	{
		LastMovementInput.X = LastMovementInput.X * 6.f / 9.f;
	}
	AddMovementInput(GetActorForwardVector() * LastMovementInput.X);
}

void ACSCharacter::MoveRight(float Value)
{
	Value *= bSprinting && !bIsCrouched ? 0.2 : bWalking ? WalkVelocityModifier : RunVelocityModifier;
	LastMovementInput.Y = Value;
	if (!bSprinting && FMath::Abs(LastMovementInput.X) > 0.f)
	{
		LastMovementInput.Y = LastMovementInput.Y * 6.f / 9.f;
	}
	AddMovementInput(GetActorRightVector() * LastMovementInput.Y);
}

void ACSCharacter::StartCrouch()
{
	if (!GetCharacterMovement()->IsFalling() && !GetCharacterMovement()->IsCrouching())
	{
		ACharacter::Crouch();
	}
}

void ACSCharacter::StopCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		ACharacter::UnCrouch();
	}
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

void ACSCharacter::Jump()
{
	if (bCanJump)
	{
		Super::Jump();
		bCanJump = false;

		FTimerHandle TimerHandle_JumpReset;
		GetWorldTimerManager().SetTimer(TimerHandle_JumpReset, this, &ACSCharacter::ResetCanJump, JumpResetTime);
	}
}

void ACSCharacter::ResetCanJump()
{
	bCanJump = true;
}

void ACSCharacter::Fire()
{
	if (Weapon)
	{
		Weapon->Fire();
	}
}

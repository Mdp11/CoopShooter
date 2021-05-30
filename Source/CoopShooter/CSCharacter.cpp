// © 2021 Mattia De Prisco All Rights Reserved


#include "CSCharacter.h"

#include "CoopShooter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CSBaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/CSHealthComponent.h"

ACSCharacter::ACSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation({0.f, 0.f, 80.f});

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UCSHealthComponent>("HealthComponent");

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 58.f;

	bWalking = false;
	bSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = 900.f;

	WalkVelocityModifier = 0.33f;
	RunVelocityModifier = 0.66f;

	JumpResetTime = 1.2f;
	bCanJump = true;

	CurrentWeaponIndex = -1;

	bTargeting = false;

	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20.f;

	WeaponAttachSocketName = "WeaponSocket";

	bIsSwitchingWeapon = false;
	bWantsToFire = false;

	HealthComponent->OnHealthChanged.AddDynamic(this, &ACSCharacter::OnHealthChanged);
}

void ACSCharacter::AddCrosshairWidget()
{
	if (CrosshairWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			CrosshairWidget = CreateWidget(PC, CrosshairWidgetClass, TEXT("Crosshair"));
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
				UCSCrosshairWidget* SpawnedWidget = Cast<UCSCrosshairWidget>(CrosshairWidget);
				if (SpawnedWidget)
				{
					SpawnedWidget->SetOwnerCharacter(this);
				}
			}
		}
	}
}

void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddCrosshairWidget();

	BaseFOV = CameraComponent->FieldOfView;

	if (WeaponClasses.Num() > 0)
	{
		Weapons.Init(nullptr, WeaponClasses.Num());
		RequestWeaponSwitch(0);
	}
}

void ACSCharacter::HandleFOV(float DeltaTime)
{
	const float TargetFOV = bTargeting ? ZoomedFOV : BaseFOV;
	const float CurrentFOV = CameraComponent->FieldOfView;

	if (!FMath::IsNearlyEqual(TargetFOV, CurrentFOV))
	{
		const float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);
		CameraComponent->SetFieldOfView(NewFOV);
	}
}

void ACSCharacter::HandleRecoil(float DeltaTime)
{
	if (!FMath::IsNearlyZero(RecoilToAdd.Pitch, 0.001f))
	{
		const float PitchRecoil = FMath::FInterpTo(0.f, RecoilToAdd.Pitch, DeltaTime, 30.f);
		RecoilToAdd.Pitch = FMath::Max(RecoilToAdd.Pitch - PitchRecoil, 0.f);
		AddControllerPitchInput(-PitchRecoil);
	}

	if (!FMath::IsNearlyZero(RecoilToAdd.Yaw, 0.001f))
	{
		const float YawRecoil = FMath::FInterpTo(0.f, RecoilToAdd.Pitch, DeltaTime, 30.f);
		RecoilToAdd.Yaw = FMath::Max(RecoilToAdd.Yaw - YawRecoil, 0.f);
		AddControllerYawInput(YawRecoil);
	}
}

void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleFOV(DeltaTime);

	HandleRecoil(DeltaTime);
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACSCharacter::RequestStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACSCharacter::RequestStopFire);

	PlayerInputComponent->BindAction<FDelegate_WeaponSwitch>("WeaponSlot1", IE_Pressed, this,
	                                                         &ACSCharacter::RequestWeaponSwitch, 0);
	PlayerInputComponent->BindAction<FDelegate_WeaponSwitch>("WeaponSlot2", IE_Pressed, this,
	                                                         &ACSCharacter::RequestWeaponSwitch, 1);
	PlayerInputComponent->BindAction<FDelegate_WeaponSwitch>("WeaponSlot3", IE_Pressed, this,
	                                                         &ACSCharacter::RequestWeaponSwitch, 2);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACSCharacter::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACSCharacter::UnZoom);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ACSCharacter::RequestReload);
}

void ACSCharacter::MoveForward(float Value)
{
	Value *= bSprinting && !bIsCrouched ? 1.f : bWalking || bTargeting ? WalkVelocityModifier : RunVelocityModifier;
	LastMovementInput.X = Value;
	if (!bSprinting && FMath::Abs(LastMovementInput.Y) > 0.f)
	{
		LastMovementInput.X = LastMovementInput.X * 6.f / 9.f;
	}
	AddMovementInput(GetActorForwardVector() * LastMovementInput.X);
}

void ACSCharacter::MoveRight(float Value)
{
	Value *= bSprinting && !bIsCrouched ? 0.2 : bWalking || bTargeting ? WalkVelocityModifier : RunVelocityModifier;
	LastMovementInput.Y = Value;
	if (!bSprinting && FMath::Abs(LastMovementInput.X) > 0.f)
	{
		LastMovementInput.Y = LastMovementInput.Y * 6.f / 9.f;
	}
	AddMovementInput(GetActorRightVector() * LastMovementInput.Y);
}

void ACSCharacter::StartCrouch()
{
	bSprinting = false;
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

void ACSCharacter::RequestStartFire()
{
	bWantsToFire = true;

	if (CurrentWeapon && !bIsSwitchingWeapon)
	{
		bSprinting = false;
		CurrentWeapon->RequestStartFire();
	}
}

void ACSCharacter::RequestStopFire()
{
	bWantsToFire = false;

	if (CurrentWeapon)
	{
		CurrentWeapon->RequestStopFire();
	}
}

ACSBaseWeapon* ACSCharacter::SpawnWeapon(const int Index)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACSBaseWeapon* Weapon = GetWorld()->SpawnActor<ACSBaseWeapon>(WeaponClasses[Index], SpawnParameters);
	if (Weapon)
	{
		Weapon->SetActorHiddenInGame(true);

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                          WeaponAttachSocketName);
		Weapon->SetOwner(this);
	}
	return Weapon;
}

void ACSCharacter::RequestWeaponSwitch(const int Index)
{
	if (CurrentWeaponIndex != Index && Index < WeaponClasses.Num())
	{
		bIsSwitchingWeapon = true;

		if (!Weapons[Index])
		{
			auto* SpawnedWeapon = SpawnWeapon(Index);

			if (!SpawnedWeapon)
			{
				return;
			}

			Weapons[Index] = SpawnedWeapon;
		}

		if (CurrentWeapon)
		{
			CurrentWeapon->UnEquip();

			ACSBaseWeapon* PreviousWeapon = CurrentWeapon;
			CurrentWeapon = nullptr;

			const float EquipAnimDuration = PlayWeaponAnimation(EquipAnim, 3.f);
			GetWorldTimerManager().SetTimer(TimerHandle_SwitchWeapon,
			                                FTimerDelegate::CreateUObject(
				                                this, &ACSCharacter::SwitchWeapon, Index, PreviousWeapon,
				                                EquipAnimDuration / 2.f),
			                                FMath::Max(0.1f, EquipAnimDuration / 2.f), false);
		}
		else
		{
			CurrentWeapon = Weapons[Index];
			CurrentWeapon->SetActorHiddenInGame(false);

			CurrentWeaponIndex = Index;

			bIsSwitchingWeapon = false;
		}
	}
}

void ACSCharacter::SwitchWeapon(const int Index, ACSBaseWeapon* PreviousWeapon, const float RemainingAnimDuration)
{
	PreviousWeapon->SetActorHiddenInGame(true);
	CurrentWeapon = Weapons[Index];
	CurrentWeapon->SetActorHiddenInGame(false);

	CurrentWeaponIndex = Index;

	GetWorldTimerManager().SetTimer(TimerHandle_CompleteWeaponSwitch, this, &ACSCharacter::CompleteWeaponSwitch,
	                                FMath::Max(0.1f, RemainingAnimDuration));
}

void ACSCharacter::CompleteWeaponSwitch()
{
	bIsSwitchingWeapon = false;
	if (bWantsToFire)
	{
		RequestStartFire();
	}
}

void ACSCharacter::Zoom()
{
	bTargeting = true;
	bSprinting = false;
}

void ACSCharacter::UnZoom()
{
	bTargeting = false;
}

bool ACSCharacter::IsFiring() const
{
	return CurrentWeapon && CurrentWeapon->IsFiring();
}

void ACSCharacter::RequestReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->RequestReload();
	}
}

void ACSCharacter::Die()
{
	bIsDead = true;
	if (CrosshairWidget)
	{
		CrosshairWidget->RemoveFromParent();
	}
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.f);
}

void ACSCharacter::OnHealthChanged(UCSHealthComponent* HealthComp, float Health, float HealthDelta,
                                   const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bIsDead && Health <= 0.f)
	{
		Die();
	}
}

float ACSCharacter::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate, FName StartSectionName)
{
	float Duration = 0.0f;

	if (Animation)
	{
		Duration = ACharacter::PlayAnimMontage(Animation, InPlayRate, StartSectionName);
	}

	return Duration / InPlayRate;
}

void ACSCharacter::AddRecoil(const FRotator& Recoil)
{
	RecoilToAdd = Recoil;
}

int ACSCharacter::GetCurrentWeaponAmmo() const
{
	return CurrentWeapon ? CurrentWeapon->GetCurrentMagazineAmmo() : 0;
}

int ACSCharacter::GetCurrentWeaponMaxAmmo() const
{
	return CurrentWeapon ? CurrentWeapon->GetCurrentBackupAmmo() : 0;
}

FVector ACSCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

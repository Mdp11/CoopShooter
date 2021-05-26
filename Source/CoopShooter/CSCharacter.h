// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "CSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACSBaseWeapon;

UCLASS()
class COOPSHOOTER_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACSCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACSBaseWeapon> WeaponClass;

	UPROPERTY()
	ACSBaseWeapon* Weapon;

	UPROPERTY(EditInstanceOnly, Category="Movement")
	float WalkVelocityModifier;

	UPROPERTY(EditInstanceOnly, Category="Movement")
	float RunVelocityModifier;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bWalking;

	bool bSprinting;

	bool bCanJump;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float JumpResetTime;

	FVector LastMovementInput;

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartCrouch();

	void StopCrouch();

	void SwitchWalkRun();

	void ActivateSprint();

	void DeactivateSprint();

	virtual void Jump() override;

	void ResetCanJump();

	void Fire();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

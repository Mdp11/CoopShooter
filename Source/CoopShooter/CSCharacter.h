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

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TArray<TSubclassOf<ACSBaseWeapon>> WeaponClasses;

	UPROPERTY()
	TArray<ACSBaseWeapon*> Weapons;

	UPROPERTY()
	ACSBaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float ZoomedFOV;

	float BaseFOV;

	UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(ClampMin = 0.1f, ClampMax = 100.f))
	float ZoomInterpSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bTargeting;

	int CurrentWeaponIndex;

	UPROPERTY(EditInstanceOnly, Category="Movement")
	float WalkVelocityModifier;

	UPROPERTY(EditInstanceOnly, Category="Movement")
	float RunVelocityModifier;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bWalking;

	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bSprinting;

	bool bCanJump;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float JumpResetTime;

	FVector LastMovementInput;

	UPROPERTY(VisibleDefaultsOnly, Category="Weapons")
	FName WeaponAttachSocketName;

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
	ACSBaseWeapon* SpawnWeapon(int Index);

	void SwitchWeapon(int Index);

	DECLARE_DELEGATE_OneParam(FDelegate_WeaponSwitch, int);

	void Zoom();

	void UnZoom();


public:
	virtual FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

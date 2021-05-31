// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "CoopShooter/UI/CSCrosshairWidget.h"
#include "GameFramework/Character.h"

#include "CSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACSWeaponBase;
class UCSHealthComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCSHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TArray<TSubclassOf<ACSWeaponBase>> WeaponClasses;

	UPROPERTY(Replicated)
	TArray<ACSWeaponBase*> Weapons;

	UPROPERTY(BlueprintReadOnly, Replicated)
	ACSWeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float ZoomedFOV;

	float BaseFOV;

	UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(ClampMin = 0.1f, ClampMax = 100.f))
	float ZoomInterpSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bTargeting;

	UPROPERTY(Replicated)
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

	FRotator RecoilToAdd;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UCSCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float NoAnimEquipDuration;

	FTimerHandle TimerHandle_SwitchWeapon;

	FTimerHandle TimerHandle_CompleteWeaponSwitch;

	bool bIsSwitchingWeapon;

	bool bWantsToFire;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead;

	virtual void BeginPlay() override;

	void HandleFOV(float DeltaTime);

	void HandleRecoil(float DeltaTime);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartCrouch();

	void StopCrouch();

	void SwitchWalkRun();

	void ActivateSprint();

	void DeactivateSprint();

	virtual void Jump() override;

	void ResetCanJump();

	void RequestStartFire();

	void RequestStopFire();

	ACSWeaponBase* SpawnWeapon(int Index);

	void RequestWeaponSwitch(int Index);

	void SwitchWeapon(int Index, ACSWeaponBase* PreviousWeapon, const float RemainingAnimDuration);

	void CompleteWeaponSwitch();

	DECLARE_DELEGATE_OneParam(FDelegate_WeaponSwitch, int);

	void Zoom();

	void UnZoom();

	UFUNCTION(BlueprintCallable, Category="State")
	bool IsFiring() const;

	void RequestReload();

	void Die();

	void AddCrosshairWidget();

	UFUNCTION()
	void OnHealthChanged(UCSHealthComponent* HealthComp, float Health,
	                     float HealthDelta, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

public:
	virtual FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddRecoil(const FRotator& Recoil);

	UFUNCTION(BlueprintCallable, Category="Ammo")
	int GetCurrentWeaponAmmo() const;

	UFUNCTION(BlueprintCallable, Category="Ammo")
	int GetCurrentWeaponMaxAmmo() const;

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f,
	                          FName StartSectionName = NAME_None);
};

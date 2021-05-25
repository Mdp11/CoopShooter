// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "CSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	DECLARE_DELEGATE_OneParam(FCrouchDelegate, bool);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

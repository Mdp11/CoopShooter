// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSCharacter.generated.h"

UCLASS()
class COOPSHOOTER_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACSCharacter();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

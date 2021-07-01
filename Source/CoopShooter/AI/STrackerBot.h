// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class COOPSHOOTER_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

protected:

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	FVector NextPathPoint;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementForce;

	UPROPERTY(EditAnywhere, Category="Movement")
	bool bUseVelocityChange;

	UPROPERTY(EditAnywhere, Category="Movement")
	float RequiredDistanceToTarget;

	FVector GetNextPathPoint() const;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};

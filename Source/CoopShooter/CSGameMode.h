// © 2021 Mattia De Prisco All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CSGameMode.generated.h"

UCLASS()
class COOPSHOOTER_API ACSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACSGameMode();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	FTimerHandle TimerHandle_BotSpawner;

	UPROPERTY(EditAnywhere, Category="WaveManagement")
	float BotsSpawnRate;

	UPROPERTY(EditAnywhere, Category="WaveManagement")
	float TimeBetweenWaves;
	
	int NumberOfBotsToSpawn;

	int WaveCount;

public:
	virtual void StartPlay() override;
};

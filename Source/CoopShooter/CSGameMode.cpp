// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGameMode.h"

ACSGameMode::ACSGameMode()
{
	BotsSpawnRate = 1.f;
	WaveCount = 0;
	NumberOfBotsToSpawn = 0;
	TimeBetweenWaves = 10.f;
}

void ACSGameMode::SpawnBotTimerElapsed()
{
	SpawnBot();
	
	if(--NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ACSGameMode::StartWave()
{
	WaveCount++;
	
	NumberOfBotsToSpawn = 2 * WaveCount;
	
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ACSGameMode::SpawnBotTimerElapsed, BotsSpawnRate, true, 0.f);
}

void ACSGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	PrepareForNextWave();
}

void ACSGameMode::PrepareForNextWave()
{
	FTimerHandle TimerHandle_StartWave;
	GetWorldTimerManager().SetTimer(TimerHandle_StartWave, this, &ACSGameMode::StartWave, TimeBetweenWaves, false);
}

void ACSGameMode::StartPlay()
{
	Super::StartPlay();
	
	PrepareForNextWave();
}

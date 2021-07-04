// © 2021 Mattia De Prisco All Rights Reserved


#include "CSGameMode.h"

#include "EngineUtils.h"
#include "Components/CSHealthComponent.h"

ACSGameMode::ACSGameMode()
{
	BotsSpawnRate = 1.f;
	WaveCount = 0;
	NumberOfBotsToSpawn = 0;
	TimeBetweenWaves = 5.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}

void ACSGameMode::SpawnBotTimerElapsed()
{
	SpawnBot();

	if (--NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ACSGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ACSGameMode::SpawnBotTimerElapsed, BotsSpawnRate,
	                                true, 0.f);
}

void ACSGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
}

void ACSGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_StartWave, this, &ACSGameMode::StartWave, TimeBetweenWaves, false);
}

void ACSGameMode::CheckWaveState()
{
	if (NumberOfBotsToSpawn > 0 || GetWorldTimerManager().IsTimerActive(TimerHandle_StartWave))
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* Pawn = *It;

		if (!Pawn || Pawn->IsPlayerControlled())
		{
			continue;
		}

		if (UCSHealthComponent* HealthComponent = Cast<UCSHealthComponent>(
			Pawn->GetComponentByClass(UCSHealthComponent::StaticClass())))
		{
			if (HealthComponent->GetHealth() > 0.f)
			{
				bIsAnyBotAlive = true;
				break;
			}
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}
}

void ACSGameMode::CheckAnyPlayerAlive()
{
	for( FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				if (UCSHealthComponent* HealthComponent = Cast<UCSHealthComponent>(
					Pawn->GetComponentByClass(UCSHealthComponent::StaticClass())))
				{
					if(HealthComponent->GetHealth() > 0.f)
					{
						return;
					}
				}
			}
		}
	}

	GameOver();
}

void ACSGameMode::GameOver()
{
	EndWave();
}

void ACSGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ACSGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

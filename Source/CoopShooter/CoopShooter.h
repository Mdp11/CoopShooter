// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define SURFACE_FLASH_DEFAULT SurfaceType1
#define SURFACE_FLASH_VULNERABLE SurfaceType2

#define COLLISION_WEAPON ECC_GameTraceChannel1

#define LOG(LogMsg) UE_LOG(LogTemp, Warning, TEXT(LogMsg))
#define LOGF(LogMsg, Float) UE_LOG(LogTemp, Warning, TEXT(LogMsg), Float)
#define LOGS(LogMsg, String) UE_LOG(LogTemp, Warning, TEXT(LogMsg), *String)


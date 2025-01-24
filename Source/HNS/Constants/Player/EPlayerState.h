// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EPlayerState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerState : uint8 {
	IDLE = 0,
	MOVING = 1,
	JUMPING = 2,
	Falling = 3,
	RUNNING = 4,
	CROUCHING = 5,
	CLIMBING = 6,
	HIDING = 7,
	ABILITY_ACTIVE = 8,
	PICKING_UP = 9,
	THROWING = 10,
	INTERACTING = 11,
	BLOCK_ANIMATION = 12,
	CASTING,
};

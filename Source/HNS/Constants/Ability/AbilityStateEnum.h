// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAbilityStateEnum : uint8 {
	READY = 0,
	ACTIVE = 1,
	CANCELLED = 2,
	COOLDOWN = 3
};

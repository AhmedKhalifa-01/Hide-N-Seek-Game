// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"

/**
 *
 */
UENUM(BlueprintType)
enum class ESpellCastingStatus_Enum : uint8 {
	CASTING_PROGRASS = 0,
	CASTING_COMPLETED = 1,
	CASTING_FAILED = 2
};

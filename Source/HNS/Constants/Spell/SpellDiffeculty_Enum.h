// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESpellDiffeculty_Enum : uint8 {
	EASY = 0,
	NORMAL = 1,
	HARD = 2
};

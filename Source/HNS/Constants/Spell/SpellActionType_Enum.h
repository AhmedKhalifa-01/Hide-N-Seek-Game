// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESpellActionType_Enum : uint8 {
	OPEN = 0,
	SELF= 1,
	OTHERS = 2,
	ENVIRONMENT = 3
};
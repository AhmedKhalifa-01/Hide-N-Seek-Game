// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellActionBase.h"
#include "../Environment/Door_Base.h"
#include "OpenDoor_SpellAction.generated.h"

/**
 * 
 */

class ACharacterBase;

UCLASS()
class HNS_API UOpenDoor_SpellAction : public USpellActionBase
{
    GENERATED_BODY()

public:
    UOpenDoor_SpellAction();
    bool Action(AActor* caster, bool bIsEnvironment) override;
};
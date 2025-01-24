// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellActionBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HNS_API USpellActionBase : public UObject
{
	GENERATED_BODY()
public:
	USpellActionBase();

protected:
		float actionCost;
	
public:
	virtual bool Action(AActor* caster, bool bIsEnvironment) PURE_VIRTUAL(USpellActionBase::Action, return false; );

	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE float getActionCost() const { return actionCost; }
};

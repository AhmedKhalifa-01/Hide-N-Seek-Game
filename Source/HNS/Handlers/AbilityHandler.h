// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityHandler.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class HNS_API UAbilityHandler : public UObject
{
	GENERATED_BODY()

public:
	UAbilityHandler();

private:

	ACharacterBase* pOwner;
	float currentCooldown;
	float maxCooldown;
	UWorld* world;
	FTimerHandle coolDownHandler;

public:
	bool canUseAbility();

	void setOwner(ACharacterBase* owner);

	void startCooldown();

	void updateCooldown();
};

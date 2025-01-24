// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "../Constants/Ability/AbilityStateEnum.h"
#include "AbilityBase.generated.h"

class USoundBase;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class HNS_API UAbilityBase : public UObject
{
	GENERATED_BODY()
public:
	UAbilityBase();

private:
	/** Ability's name  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	FString abilityName;

	/** Ability's Max cooldown  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	float maxCooldown;

	/** Ability's current cooldown  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	float currentCooldown;

	/** Ability's Max duration*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	float maxDuration;

	/** Ability's current Duration  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	float currentDuration;

	/** Ability's Max uses*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	int maxUses;

	/** Ability's current uses*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	int currentUses;

	/** Ability's sound effects*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> soundEffects;

	/** Ability's particle effects*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystem*> particleEffects;

	/** Ability's state*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	EAbilityStateEnum state;

};

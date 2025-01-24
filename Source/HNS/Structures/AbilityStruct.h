// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "../Constants/Ability/AbilityStateEnum.h"
#include "AbilityStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HNS_API FAbilityStruct
{
	GENERATED_BODY()
public:
	FAbilityStruct();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
		TArray<USoundBase*> soundEffects;

	/** Ability's particle effects*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
		TArray<UParticleSystem*> particleEffects;

	/** Ability's state*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
		EAbilityStateEnum state;

	/** Ability's thumbnail*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
		UTexture2D* thumbnail;
};

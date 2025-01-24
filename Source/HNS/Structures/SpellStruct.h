// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "../Constants/Spell/SpellDiffeculty_Enum.h"
#include "../Constants/Spell/SpellActionType_Enum.h"
#include "../Spell Action/SpellActionBase.h"
#include "SpellStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HNS_API FSpellStruct
{
	GENERATED_BODY()
public:
	FSpellStruct();

	/** spell's name  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		FString spellName;

	/** spell's description  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		FString spellDescription;

	/** spell's diffeculty  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		ESpellDiffeculty_Enum spellDiffeculty;

	/** spell's words  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		FString spellWords;

	/** spell's allowed cast time  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		float castDuration;

	/** spell's performed action  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		USpellActionBase* spellAction;

	/** spell's action type*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Spell, meta = (AllowPrivateAccess = "true"))
		ESpellActionType_Enum spellActionType;
};

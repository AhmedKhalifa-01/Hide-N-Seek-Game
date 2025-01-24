// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structures/SpellStruct.h"
#include "Spell Action/OpenDoor_SpellAction.h"
#include "Spell Action/CloseDore_SpellAction.h"
#include "HNSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HNS_API AHNSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHNSGameModeBase();

private:
	/*The list of spells in the game*/
	TArray<FSpellStruct> spellsList;

public:
	UFUNCTION(BlueprintCallable, Category = "Spell")
		TArray<FSpellStruct> getSpellsList();

	UFUNCTION(BlueprintCallable, Category = "Spell")
		void generateSpells();
	
};

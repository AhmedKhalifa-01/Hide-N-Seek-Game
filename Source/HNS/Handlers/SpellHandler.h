// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Structures/SpellStruct.h"
#include "../Constants/Spell/SpellCastingStatus_Enum.h"
#include "../HNSGameModeBase.h"
#include "SpellHandler.generated.h"

/**
 * 
 */


class SpellStack {

public:
	SpellStack();
	int top;
	int capacity;
	TArray<FSpellStruct> spell_Stack;

	void push(FSpellStruct spellS);
	void pop();
	bool isEmpty();

};


UCLASS(Blueprintable)
class HNS_API USpellHandler : public UObject
{
	GENERATED_BODY()

public:
	USpellHandler();

private:
	int letterIndex;
	FSpellStruct spell;
	bool isSpellSet;

public:
	TArray<FSpellStruct> spellsList;

	//TArray<FSpellStruct> castingSpellMatch;

	FString c_spell;

	SpellStack * ss;

	void testSpells();

	AActor* owner;

	void setOwner(AActor* newOwner);

	ESpellCastingStatus_Enum castSpell(FString letter);

	FSpellStruct getSpell();

	UFUNCTION(BlueprintCallable, Category = "spell")
		void spellCastLimitFailed();

	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE bool getIsSpellSet() const { return isSpellSet; };

	UFUNCTION(BlueprintCallable, Category = "spell")
		void setSpell(FSpellStruct newSpell);

	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE bool getSpellPerformedAction() const { return spellPerformedAction; }

private:
	bool spellPerformedAction;
};

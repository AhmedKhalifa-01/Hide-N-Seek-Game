// Copyright Epic Games, Inc. All Rights Reserved.

#include "HNSGameModeBase.h"

AHNSGameModeBase::AHNSGameModeBase() {
	generateSpells();
}

TArray<FSpellStruct> AHNSGameModeBase::getSpellsList()
{
	return spellsList;
}

void AHNSGameModeBase::generateSpells() {

	spellsList.Empty(); // Clear the array before adding new elements
	FSpellStruct newSpell;

	// Adding close door spell
	newSpell.spellName = "Close Door";
	newSpell.spellDescription = "Used to close any door in 2 feet range";
	newSpell.spellDiffeculty = ESpellDiffeculty_Enum::EASY;
	newSpell.spellWords = "closedoor";
	newSpell.castDuration = 5;
	newSpell.spellActionType = ESpellActionType_Enum::OPEN;
	newSpell.spellAction = NewObject<UCloseDoor_SpellAction>();
	spellsList.Add(newSpell);

	// Adding open door spell
	newSpell.spellName = "Open Door";
	newSpell.spellDescription = "Used to open any door in 2 feet range";
	newSpell.spellDiffeculty = ESpellDiffeculty_Enum::EASY;
	newSpell.spellWords = "opendoor";
	newSpell.castDuration = 5;
	newSpell.spellActionType = ESpellActionType_Enum::OPEN;
	newSpell.spellAction = NewObject<UOpenDoor_SpellAction>();
	spellsList.Add(newSpell);
}


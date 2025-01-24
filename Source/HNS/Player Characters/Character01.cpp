// Fill out your copyright notice in the Description page of Project Settings.


#include "Character01.h"

ACharacter01::ACharacter01() {
	myAbility.abilityName = "Rage Storm";
	myAbility.state = EAbilityStateEnum::READY;
	myAbility.maxCooldown = 5;
	spellPower = 20;
	maxSpellPower = 20;
}
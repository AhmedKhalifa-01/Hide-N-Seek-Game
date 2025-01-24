// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityHandler.h"
#include "../Player Characters/CharacterBase.h"
#include "Engine/World.h"
#include "../Abilities/AbilityBase.h"

UAbilityHandler::UAbilityHandler()
{
}

void UAbilityHandler::setOwner(ACharacterBase* owner) {
	this->pOwner = owner;
	world = pOwner->GetWorld();
}

void UAbilityHandler::startCooldown() {
	maxCooldown = pOwner->getAbility().maxCooldown;
	currentCooldown = maxCooldown;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MAX Cool down : %f"), maxCooldown));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Starting cool down")));
	pOwner->changeAbilityState(EAbilityStateEnum::COOLDOWN);
	world->GetTimerManager().SetTimer(coolDownHandler, this, &UAbilityHandler::updateCooldown, 1.0f, true);
}

void UAbilityHandler::updateCooldown() {
	if (currentCooldown > 0) {
		currentCooldown -= 1;
		pOwner->changeAbilityCurrentCooldown(currentCooldown);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Cool down : %f"), currentCooldown));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Cool down : %f"), currentCooldown));
		world->GetTimerManager().ClearTimer(coolDownHandler);
		pOwner->changeAbilityState(EAbilityStateEnum::READY);
	}
	
}

bool UAbilityHandler::canUseAbility() {
	if (pOwner->getEPlayerState() == EPlayerState::BLOCK_ANIMATION
		|| pOwner->getEPlayerState() == EPlayerState::CROUCHING
		|| pOwner->getEPlayerState() == EPlayerState::HIDING
		|| pOwner->getEPlayerState() == EPlayerState::INTERACTING
		|| pOwner->getEPlayerState() == EPlayerState::PICKING_UP
		|| pOwner->getEPlayerState() == EPlayerState::Falling) 
	{
		UE_LOG(LogTemp, Log, TEXT("Player state fail"));
		return false;
	}
	FAbilityStruct m_ability = pOwner->getAbility();
	if (m_ability.abilityName != "" && m_ability.state == EAbilityStateEnum::READY) {
		return true;
	}
	UE_LOG(LogTemp, Log, TEXT("Ability failed"));
	return false;
}


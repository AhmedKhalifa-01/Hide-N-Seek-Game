// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseDore_SpellAction.h"
#include "../Player Characters/CharacterBase.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

UCloseDoor_SpellAction::UCloseDoor_SpellAction() {
	this->actionCost = 10;
}
bool UCloseDoor_SpellAction::Action(AActor* caster, bool bIsEnvironment)
{

	ACharacterBase* playerRef = Cast<ACharacterBase>(caster);
	if (playerRef) {
		if (playerRef->getSpellPower() < actionCost) {
			return false;
		}
	}

	// Implementation of the Action function in the derived class
	UE_LOG(LogTemp, Error, TEXT("Implementing the Close door MF !"));

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(caster, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	// Total radius of the sphere
	float radius = 200.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = caster->GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = AActor::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(caster->GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	DrawDebugSphere(caster->GetWorld(), caster->GetActorLocation(), radius, 12, FColor::Magenta, true, 3.0f);
	// Finally iterate over the outActor array
	for (AActor* actor : outActors) {
		if (actor->IsA<ADoor_Base>()) {
			if (Cast<ADoor_Base>(actor)) {
				if (!(Cast<ADoor_Base>(actor)->getIsOpen())) {
					UE_LOG(LogTemp, Error, TEXT("Door already closed"));
					return false;
				}
			}
			IInteractInterface* Interface = Cast<IInteractInterface>(actor);
			if (Interface) {
				// Excuting the interacte interface to close the door
				Interface->Execute_Interact(actor, Cast<ACharacterBase>(caster));
				return true;
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("Does not implement the interact interface"));
			}
		}

	}
	return false;
}




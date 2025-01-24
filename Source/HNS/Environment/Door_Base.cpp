// Fill out your copyright notice in the Description page of Project Settings.


#include "Door_Base.h"
#include "../Player Characters/CharacterBase.h"
#include <Runtime/Core/Public/Misc/OutputDeviceNull.h>

// Sets default values
ADoor_Base::ADoor_Base()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    //Create the mesh component
    doorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Door Mesh"));
    CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Mesh"));

    // Set the size and position of the collision box.
    CollisionMesh->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    CollisionMesh->SetCollisionProfileName("Trigger");
    CollisionMesh->SetHiddenInGame(false);
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionMesh->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionMesh->SetGenerateOverlapEvents(true);


    RootComponent = CollisionMesh;
    doorMesh->SetupAttachment(CollisionMesh);

}
void ADoor_Base::Interact_Implementation(ACharacterBase* playerRef) {

    /*FString FunctionNameWithArgument = FString::Printf(TEXT("openingDoor %s"), playerRef);

    FOutputDeviceNull ar;

    this->CallFunctionByNameWithArguments(*FunctionNameWithArgument, ar, nullptr, true);*/
}

void ADoor_Base::setIsOpen(bool newIsOpen) {
    bIsOpen = newIsOpen;
}

void ADoor_Base::setIsInProccess(bool newIsOnProccess) {
    bIsInProccess = newIsOnProccess;
}
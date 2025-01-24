// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
// Sets default values
APickupBase::APickupBase(int x) {

}
APickupBase::APickupBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    //Create the mesh component
    pickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
    CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Mesh"));

    // Set the size and position of the collision box.
    CollisionMesh->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    CollisionMesh->SetCollisionProfileName("Trigger");
    CollisionMesh->SetHiddenInGame(false);
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionMesh->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionMesh->SetGenerateOverlapEvents(true);


    RootComponent = CollisionMesh;
    pickupMesh->SetupAttachment(CollisionMesh);

    

    // Load the texture asset
    static ConstructorHelpers::FObjectFinder<UTexture2D> TextureAsset(TEXT("/Game/UI/Images/img01.img01"));
    if (TextureAsset.Succeeded())
    {
        // Set the texture reference
        thumpnail = TextureAsset.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture asset!"));
    }
    //CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);

}

void APickupBase::Interact_Implementation(ACharacterBase* playerRef) {
    if (playerRef) {
        UE_LOG(LogTemp, Log, TEXT("Picked up by: %s"), *playerRef->GetName());
        FTimerHandle Handle;
        FTimerDelegate Delegate;
        playerRef->playPickupAnimation();
        APickupBase* newPickup = NewObject<APickupBase>();
        newPickup->setThumbnail(this->thumpnail);
        playerRef->setPickup(newPickup);
        Destroy();
    }
}
void APickupBase::destroyMe() {
    UE_LOG(LogTemp, Log, TEXT("NOOOOOOOOOOOOOOOOO"));
    //this->Destroy();
}

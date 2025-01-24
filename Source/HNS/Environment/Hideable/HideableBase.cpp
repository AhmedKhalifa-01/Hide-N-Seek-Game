// Fill out your copyright notice in the Description page of Project Settings.


#include "HideableBase.h"


// Sets default values
AHideableBase::AHideableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the mesh component
    HideableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hideable Mesh"));
    CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Mesh"));

    // Set the size and position of the collision box.
    CollisionMesh->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    CollisionMesh->SetCollisionProfileName("Trigger");
    CollisionMesh->SetHiddenInGame(false);
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionMesh->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionMesh->SetGenerateOverlapEvents(true);


    RootComponent = CollisionMesh;
    HideableMesh->SetupAttachment(CollisionMesh);

    CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AHideableBase::OnOverlapBegin);

    

}

// Called when the game starts or when spawned
void AHideableBase::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("Begin"));
}

// Called every frame
void AHideableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHideableBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *OtherActor->GetName()));
}

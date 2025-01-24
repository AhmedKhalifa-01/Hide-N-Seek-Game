// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/InteractInterface.h"
#include "../../Player Characters/CharacterBase.h"
#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include "PickupBase.generated.h"


UCLASS()
class HNS_API APickupBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();
	APickupBase(int x);
	// Getter and setter for the thumbnail variable
	UFUNCTION(BlueprintPure)
		UTexture2D* getThumbnail() const { return thumpnail; }
	UFUNCTION(BlueprintCallable)
		void setThumbnail(UTexture2D* NewThumbnail) { thumpnail = NewThumbnail; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* pickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
		UTexture2D* thumpnail;

	void Interact_Implementation(ACharacterBase* playerRef) override;

	void destroyMe();
};

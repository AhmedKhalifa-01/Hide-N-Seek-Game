// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/InteractInterface.h"
#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include "Door_Base.generated.h"

class ACharacterBase;
UCLASS()
class HNS_API ADoor_Base : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor_Base();

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* doorMesh;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionMesh;

	void Interact_Implementation(ACharacterBase* playerRef) override;

private:
	bool bIsOpen;

	bool bIsInProccess;

public:
	UFUNCTION(BlueprintCallable, Category = "Door")
		FORCEINLINE bool getIsInProccess() const { return bIsInProccess; }

	UFUNCTION(BlueprintCallable, Category = "Door")
		FORCEINLINE bool getIsOpen() const { return bIsOpen; }

	UFUNCTION(BlueprintCallable, Category = "Door")
		void setIsOpen(bool newIsOpen);

	UFUNCTION(BlueprintCallable, Category = "Door")
		void setIsInProccess(bool newIsOnProccess);
};

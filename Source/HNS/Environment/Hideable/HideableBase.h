// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime/Engine/Classes/Components/BoxComponent.h>
#include "HideableBase.generated.h"


UCLASS()
class HNS_API AHideableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHideableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hideable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HideableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hideable", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionMesh;

/*FUNCTION SECTIONS*/
public:
	FORCEINLINE UStaticMeshComponent* getMesh() const { return HideableMesh; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

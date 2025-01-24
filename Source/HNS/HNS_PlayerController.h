// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HNS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HNS_API AHNS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHNS_PlayerController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
private:

};

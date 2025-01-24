// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "HNS_PlayerController.h"

AHNS_PlayerController::AHNS_PlayerController() {
}
// Called every frame
void AHNS_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}
void AHNS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Hello, world!"));
}

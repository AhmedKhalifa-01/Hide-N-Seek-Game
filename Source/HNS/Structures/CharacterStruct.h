// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HNS_API FCharacterStruct
{
	GENERATED_BODY()
public:
	FCharacterStruct();

	/** Character's name  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
		FString characterName;

	/** Character's description  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
		FString characterDescription;

	/** Character's thumbnail 01  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
		UTexture2D* characterThump01;

	/** Character's thumbnail 02  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
		UTexture2D* characterThump02;

	/** Character's thumbnail 03  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true"))
		UTexture2D* characterThump03;

	
};

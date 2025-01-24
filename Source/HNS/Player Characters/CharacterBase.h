// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Constants/Player/EPlayerState.h"
#include "../Constants/Spell/SpellCastingStatus_Enum.h"
#include "../Abilities/AbilityBase.h"
#include "../Structures/AbilityStruct.h"
#include "../Structures/CharacterStruct.h"
#include "../Structures/SpellStruct.h"
#include "../Handlers/SpellHandler.h"
#include "Engine/EngineTypes.h"
#include "InputCoreTypes.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "CharacterBase.generated.h"

class APickupBase;
class UAbilityHandler;
class AHideableBase;

UCLASS()
class HNS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	ACharacterBase();

	//void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprint Class")
		AActor* doorBP;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Standing to hiding animation montag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* standToHide_animMontag;

	/** Hiding to standing animation montag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* hideToStand_animMontag;

	/** Standing up then hiding animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* crouchToHide_animMontag;

	/** Standing up then hiding animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* openDoor_animMontag;

	/** Standing up then hiding animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* pickup_animMontag;

	/** Using abilty Animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* useAbillty_animMontag;


private:
	// how often does the spell power updates
	float spellPowerUpdateRate;

	// how much spell power is added in each update
	float spellPowerUpdateQty;

	// The timer handler to handle the timer of the spell power update
	FTimerHandle spellPower_TimerHandler;

	void updateSpell();



protected:
	/** The structure for the character's info*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
		FCharacterStruct characterData;

	/** The spell hanlder for the player*/
	USpellHandler* spellHandler;

	/** The ability hanlder for the player*/
	UAbilityHandler* abilityHandler;

	/** The ability of the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = "true"))
		FAbilityStruct myAbility;

	/** The ENUM that defines the player's state*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
		EPlayerState E_playerState;

	/** The ENUM that shows the spell cast status*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerCharacter)
		ESpellCastingStatus_Enum E_SpellCastStatus;

	

	/** The meter of detection of the player*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
		float detectionMetere;

	/** The meter of detection of the player*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
		bool isCampusActive;

	/** Whither to allow the character to move or not*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
		bool bAllowMovement;

	/** The actor in the game that the player is hiding behind*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
		AHideableBase* hideObject;

	/** The cuurent spell action power the player has */
	UPROPERTY(VisibleAnywhere, Category = spell)
		float spellPower;

	/** The maximum spell power a character could have*/
	UPROPERTY(VisibleAnywhere, Category = spell)
		float maxSpellPower;


public:
	/** The actor representing the pickup of the player*/
	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter, meta = (AllowPrivateAccess = "true"))
		APickupBase* pickup;

	/** The actor representing the pickup of the player*/
	UPROPERTY(VisibleAnywhere, Category = Hiding)
		bool bCanHide;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	//The function that handles the hiding opertation for the player
	void hide();

	//The function that handles the unhiding the player
	void unHide();

public:
	/** Returns ability**/
	FORCEINLINE FAbilityStruct getAbility() const { return myAbility; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable, Category = "Player State")
		FORCEINLINE EPlayerState getEPlayerState() const { return E_playerState; }

	/** Returns spellHandler**/
	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE USpellHandler* getSpellHandler() const { return spellHandler; }

	/* Getting the value of allowmovement variable*/
	UFUNCTION(BlueprintCallable, Category = "Player")
		FORCEINLINE bool getBAllowMovement() const { return bAllowMovement; }

	/* Getting the value of the object the player is hideing behind variable*/
	UFUNCTION(BlueprintCallable, Category = "Hiding")
		FORCEINLINE AHideableBase* getHideObject() const { return hideObject; }

	/* Getting the value of the the pick up of the player*/
	UFUNCTION(BlueprintCallable, Category = "Item")
		FORCEINLINE APickupBase* getPickup() const { return pickup; }

	/* Getting the value of the can hide*/
	UFUNCTION(BlueprintCallable, Category = "Hiding")
		FORCEINLINE bool getBCanHide() const { return bCanHide; }

	/* Getting the value of spell handler*/
	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE ESpellCastingStatus_Enum getSpellCastingStatus() const { return E_SpellCastStatus; }

	/* Getting the value of spell power*/
	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE float getSpellPower() const { return spellPower; }

	/* Getting the value of maximum spell power*/
	UFUNCTION(BlueprintCallable, Category = "spell")
		FORCEINLINE float getMaxSpellPower() const { return maxSpellPower; }





	// The SET Functions
	/*Updating the value of the player's state*/
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void setEPlayerState(EPlayerState newState);

	/*Updating the value of allowing movement*/
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void setBAllowMovement(bool allow);

	/*Updating the value of allowing movement*/
	UFUNCTION(BlueprintCallable, Category = "Hiding")
		void setBCanHide(bool newCanHide);

	/*Updating the pickedup object*/
	UFUNCTION(BlueprintCallable, Category = "Item")
		void setPickup(APickupBase* newPickup);
	// Handling hide functions
		/*Updating the object that the player is hiding behind*/
	UFUNCTION(BlueprintCallable, Category = "Hiding")
		void setHideObject(AHideableBase* newHideObject);

	UFUNCTION(BlueprintCallable, Category = "Hiding")
		bool canHideRight();

	UFUNCTION(BlueprintCallable, Category = "Hiding")
		bool canHideLeft();

	UFUNCTION(BlueprintCallable, Category = "spell")
		void setSpellPower(float newSpellPower);

	UFUNCTION(BlueprintCallable, Category = "spell")
		void setMaxSpellPower(float newMaxSpellPower);

	// Player states handling section

		/*Checking if can play an animation*/
	UFUNCTION(BlueprintCallable, Category = "Player State")
		bool canPlayAnimation();

	/*Playing an animation*/
	UFUNCTION(BlueprintCallable, Category = "Player State")
		bool playAnimation(UAnimMontage* anim, float playRate, FName name);


	/*Updating the item picked up by this player*/
	//UFUNCTION(BlueprintCallable, Category = "Player")
	//void setPickup(APickupBase* newPickup);

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void sprint();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void stopSprint();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		bool canSprint();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// CROUCHING FUNCTIONS
	bool canCrouch();

	void crouch();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void stand();

	// Interacting wwith environment
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void interact();

	//	Opening a door
	UFUNCTION(BlueprintCallable, Category = "Player State")
		bool canOpenDoor();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		bool openDoor();

	// Picking up items - Using pickups
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		bool canPickup();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		bool isPickupValid();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void playerPickup(APickupBase* newPickup);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		bool playPickupAnimation();

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void tryUsePickup();

	//
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void checkInteractable();

	UFUNCTION(BlueprintCallable, Category = "Ability")
		void tryUseAbility();

	void changeAbilityState(EAbilityStateEnum newState);

	void changeAbilityCurrentCooldown(float newCooldown);

	/*Spell functions*/
	void KeyPress(FKey key);

	void startCastingMode();

	UFUNCTION(BlueprintCallable, Category = "spell")
		FSpellStruct getSpell();

	UFUNCTION(BlueprintCallable, Category = "spell")
		void callSpellCastLimitFailed();

	UFUNCTION(BlueprintCallable, Category = "spell")
		bool callSpellPerformedAction();
	
};

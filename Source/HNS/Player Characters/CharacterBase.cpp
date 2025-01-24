// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\KismetMathLibrary.h"
#include "../Environment/Hideable/HideableBase.h"
#include "../Environment/Pickups/PickupBase.h"
#include "../Handlers/AbilityHandler.h"
#include <Runtime/Core/Public/Misc/OutputDeviceNull.h>
// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Setting the player state to default value witch is Idle
	E_playerState = EPlayerState::IDLE;

	// The default value to allow movement is true
	bAllowMovement = true;

	bCanHide = true;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACharacterBase::OnComponentEndOverlap);

	pickup = NewObject<APickupBase>();

	abilityHandler = NewObject<UAbilityHandler>();
	abilityHandler->setOwner(this);

	spellPowerUpdateRate = 3.0f;
	spellPowerUpdateQty = 1.0f;

}

/*void ACharacterBase::BeginPlay() {
	//GetWorldTimerManager().SetTimer(spellPower_TimerHandler, this, &ACharacterBase::updateSpell, spellPowerUpdateRate, true, 0.5f);
}*/
//////////////////////////////////////////////////////////////////////////
// Input

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACharacterBase::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACharacterBase::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACharacterBase::OnResetVR);

	// The hiding button for the player
	PlayerInputComponent->BindAction("Hide", IE_Pressed, this, &ACharacterBase::hide);

	// Set player to sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::sprint);
	// Set player to stop sprinting
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::stopSprint);

	// Button to crouch or stand
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacterBase::crouch);

	// Button to interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACharacterBase::checkInteractable);

	// Button to use ability
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ACharacterBase::tryUseAbility);

	// Button for using the pickup
	PlayerInputComponent->BindAction("UsePickup", IE_Pressed, this, &ACharacterBase::tryUsePickup);

	// Any key being pressed to hanlde spell casting
	PlayerInputComponent->BindAction("Any", IE_Pressed, this, &ACharacterBase::KeyPress);

	PlayerInputComponent->BindAction("SpellCastMode", IE_Pressed, this, &ACharacterBase::startCastingMode);
}


void ACharacterBase::OnResetVR()
{
	// If HideNSeek is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in HideNSeek.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACharacterBase::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACharacterBase::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACharacterBase::TurnAtRate(float Rate)
{
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::MoveForward(float Value)
{
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if (E_playerState == EPlayerState::HIDING) {
		return;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if (E_playerState == EPlayerState::HIDING)
		{
			// Determine the movement direction based on the input value
			FVector Direction = FVector::ZeroVector;
			if (Value < 0.0f && canHideLeft())
			{
				Direction = FVector(0.0f, -1.0f, 0.0f);
			}
			else if (Value > 0.0f && canHideRight())
			{
				Direction = FVector(0.0f, 1.0f, 0.0f);
			}

			// Move the player in the chosen direction
			if (Direction != FVector::ZeroVector)
			{
				AddMovementInput(Direction, 1.0f);
			}
		}
		else
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}
// The hiding function for the player
void ACharacterBase::hide() {
	if (E_playerState == EPlayerState::HIDING) {
		unHide();
		return;
	}
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}

	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	// Total radius of the sphere
	float radius = 100.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = AHideableBase::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, true, 3.0f);
	// Finally iterate over the outActor array
	for (AActor* overlappedActor : outActors) {
		AHideableBase* newHide = Cast<AHideableBase>(overlappedActor);
		if (newHide && bCanHide) {
			if (E_playerState == EPlayerState::CROUCHING) {
				if (playAnimation(crouchToHide_animMontag, 1, NAME_None)) {
					// changing the direction of the player to alligned with the hiding object
					FVector target(overlappedActor->GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
					SetActorRotation(FRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target)));
					// updating movement
					GetCharacterMovement()->bOrientRotationToMovement = false;
					GetCharacterMovement()->MaxWalkSpeed = 50;
					hideObject = newHide;
					E_playerState = EPlayerState::BLOCK_ANIMATION;
					FString ActorName = getHideObject()->GetName();
					UE_LOG(LogTemp, Warning, TEXT("Line trace hit actor %s"), *ActorName);
				}
			}
			else {
				if (playAnimation(standToHide_animMontag, 1, NAME_None)) {
					// changing the direction of the player to alligned with the hiding object
					FVector target(overlappedActor->GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
					SetActorRotation(FRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target)));
					// updating movement
					GetCharacterMovement()->bOrientRotationToMovement = false;
					GetCharacterMovement()->MaxWalkSpeed = 50;
					hideObject = newHide;
					E_playerState = EPlayerState::BLOCK_ANIMATION;
					FString ActorName = getHideObject()->GetName();
					UE_LOG(LogTemp, Warning, TEXT("Line trace hit actor %s"), *ActorName);
				}
			}
		}
		UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
	}
}
/*Unhiding the player*/
void ACharacterBase::unHide() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if (playAnimation(hideToStand_animMontag, 1, NAME_None)) {
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 100;
		E_playerState = EPlayerState::BLOCK_ANIMATION;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Un hiding bitches !"));
	}
}
//checking if character can move right while hiding
bool ACharacterBase::canHideRight() {
	if (E_playerState == EPlayerState::CASTING) {
		return false;
	}
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	// Calculate the end location of the line trace
	FVector EndLocation = StartLocation + (ActorRotation.Vector() * -20);

	// Perform the line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore the character itself
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);

	StartLocation = EndLocation;
	float EndLocation_y = EndLocation.Y;
	EndLocation_y += 30;
	EndLocation.Y = EndLocation_y;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);

	StartLocation = EndLocation;
	float EndLocation_x = EndLocation.X;
	EndLocation_x += 150;
	EndLocation.X = EndLocation_x;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);
	// Check if the line trace hit something
	if (HitResult.bBlockingHit)
	{
		// Do something with the hit result, such as print the name of the actor that was hit
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			AHideableBase* newHide = Cast<AHideableBase>(HitActor);
			if (newHide) {
				return true;
			}
		}
	}
	return false;
}
bool ACharacterBase::canHideLeft() {
	if (E_playerState == EPlayerState::CASTING) {
		return false;
	}
	FVector StartLocation = GetActorLocation();
	FRotator ActorRotation = GetActorRotation();

	// Calculate the end location of the line trace
	FVector EndLocation = StartLocation + (ActorRotation.Vector() * -20);

	// Perform the line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore the character itself
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);

	StartLocation = EndLocation;
	float EndLocation_y = EndLocation.Y;
	EndLocation_y -= 30;
	EndLocation.Y = EndLocation_y;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);

	StartLocation = EndLocation;
	float EndLocation_x = EndLocation.X;
	EndLocation_x += 150;
	EndLocation.X = EndLocation_x;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);
	if (HitResult.bBlockingHit)
	{
		// Do something with the hit result, such as print the name of the actor that was hit
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			AHideableBase* newHide = Cast<AHideableBase>(HitActor);
			if (newHide) {
				return true;
			}
		}
	}
	return false;
}
// The setters sections
/*Updating player state*/
void ACharacterBase::setEPlayerState(EPlayerState newState) {
	E_playerState = newState;
}

/*Updating Allow movement*/
void ACharacterBase::setBAllowMovement(bool allow) {
	bAllowMovement = allow;
}

/*Updating can hide*/
void ACharacterBase::setBCanHide(bool newCanHide) {
	bCanHide = newCanHide;
}
/*Updating the hideableobject*/
void ACharacterBase::setHideObject(AHideableBase* newHideObject) {
	hideObject = newHideObject;
}

/*Updating the spell power*/
void ACharacterBase::setSpellPower(float newSpellPower) {
	spellPower = newSpellPower;
	if (spellPower <= 0) {
		spellPower = 0;
	}
	if (spellPower >= maxSpellPower) {
		spellPower = maxSpellPower;
	}
}

/*Updating the max spell power*/
void ACharacterBase::setMaxSpellPower(float newMaxSpellPower) {
	maxSpellPower = newMaxSpellPower;
}

/* Handling player states*/
bool ACharacterBase::canPlayAnimation()
{
	if (E_playerState == EPlayerState::BLOCK_ANIMATION || E_playerState == EPlayerState::CASTING) {
		return false;
	}
	return true;
}

bool ACharacterBase::playAnimation(UAnimMontage* anim, float playRate, FName name)
{
	if (E_playerState == EPlayerState::CASTING) {
		return false;
	}
	if (!canPlayAnimation() || !anim) {
		return false;
	}
	PlayAnimMontage(anim, playRate, name);
	return true;
}


void ACharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (E_playerState == EPlayerState::CASTING) {
		bCanHide = false;
	}
	if (E_playerState != EPlayerState::HIDING) {
		if (Cast<AHideableBase>(OtherActor)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("My space mf!"));
			bCanHide = false;
		}
	}
}
void ACharacterBase::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (E_playerState == EPlayerState::CASTING) {
		bCanHide = false;
	}
	if (E_playerState != EPlayerState::HIDING) {
		if (Cast<AHideableBase>(OtherActor)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("leave !!!!"));
			bCanHide = true;
		}
	}

}

// Handle sprinting
void ACharacterBase::sprint() {
	if (canSprint()) {
		E_playerState = EPlayerState::RUNNING;
		GetCharacterMovement()->MaxWalkSpeed = 450;
		bCanHide = false;
	}
}

void ACharacterBase::stopSprint() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if (E_playerState == EPlayerState::HIDING) {
		return;
	}
	E_playerState = EPlayerState::IDLE;
	GetCharacterMovement()->MaxWalkSpeed = 100;
}
/* checking if player can sprint*/
bool ACharacterBase::canSprint() {

	if (E_playerState == EPlayerState::CASTING || E_playerState == EPlayerState::HIDING || E_playerState == EPlayerState::BLOCK_ANIMATION) {
		return false;
	}
	return true;
}

// Handle Crouching
/* checking if player can sprint*/
bool ACharacterBase::canCrouch() {
	if (E_playerState == EPlayerState::CASTING || E_playerState == EPlayerState::HIDING || E_playerState == EPlayerState::BLOCK_ANIMATION || E_playerState == EPlayerState::RUNNING) {
		return false;
	}
	return true;
}

void ACharacterBase::crouch() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if (E_playerState == EPlayerState::CROUCHING) {
		stand();
		return;
	}
	if (canCrouch()) {
		E_playerState = EPlayerState::CROUCHING;
		GetCharacterMovement()->MaxWalkSpeed = 100;
		bCanHide = true;
	}
}

void ACharacterBase::stand() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	E_playerState = EPlayerState::IDLE;
}

// Handle interacting with environment
void ACharacterBase::interact() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	if (pickup) {
		UE_LOG(LogTemp, Log, TEXT("my thumpnail : %s"), *pickup->getThumbnail()->GetName());
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("m no no"));

	}

	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	// Total radius of the sphere
	float radius = 100.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = AHideableBase::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, true, 3.0f);
	// Finally iterate over the outActor array
	for (AActor* overlappedActor : outActors) {
		UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
	}
}

// Handling opening a door
bool ACharacterBase::canOpenDoor() {
	if (E_playerState == EPlayerState::CASTING) {
		return false;
	}
	E_playerState = EPlayerState::IDLE;
	return true;
}

bool ACharacterBase::openDoor() {
	return true;
}

// Handling picking up
bool ACharacterBase::canPickup() {
	return true;
}

// setting the pickup actor
void ACharacterBase::setPickup(APickupBase* newPickup)
{
	pickup = newPickup;
	newPickup->Destroy();
	UE_LOG(LogTemp, Log, TEXT("my thumpnail : %s"), *pickup->getThumbnail()->GetName());
}

bool ACharacterBase::isPickupValid() {
	if (pickup != nullptr) {
		return true;
	}
	return false;
}

void ACharacterBase::playerPickup(APickupBase* newPickup) {
	if (canPickup()) {
		IInteractInterface* Interface = Cast<IInteractInterface>(newPickup);
		if (Interface) {
			Interface->Execute_Interact(newPickup, this);
		}
	}
}

bool ACharacterBase::playPickupAnimation() {
	if (playAnimation(pickup_animMontag, 3, NAME_None)) {
		return true;
	}
	return false;
}

// Trying to use the pick up 
void ACharacterBase::tryUsePickup() {
	if (pickup) {

	}
	else {
		FString ArgumentValue = TEXT("Hello, World!");

		FString FunctionNameWithArgument = FString::Printf(TEXT("displayNoItemWarning %s"), *ArgumentValue);

		FOutputDeviceNull ar;

		Controller->CallFunctionByNameWithArguments(*FunctionNameWithArgument, ar, nullptr, true);
	}
}

// Interactables Sections

void ACharacterBase::checkInteractable() {
	// Check the type of the actor and take appropriate action
	// Set what actors to seek out from it's collision channel
	if (pickup) {
		UE_LOG(LogTemp, Log, TEXT("my thumpnail : %s"), *pickup->getThumbnail()->GetName());
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("m no no"));
	}
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> outActors;

	// Total radius of the sphere
	float radius = 100.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = AActor::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Magenta, true, 3.0f);
	// Finally iterate over the outActor array
	for (AActor* actor : outActors) {
		if (actor->IsA<APickupBase>()) {
			//Check if already picked an item
			if (!pickup) {
				APickupBase* pu = Cast<APickupBase>(actor);
				playerPickup(pu);
			}
			break;
		}
		else {

			IInteractInterface* Interface = Cast<IInteractInterface>(actor);
			if (Interface) {
				Interface->Execute_Interact(actor, this);
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("Does not implement the interact interface"));
			}
		}

	}
}

// trying to use the ability
void ACharacterBase::tryUseAbility() {
	if (E_playerState == EPlayerState::CASTING) {
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Trying to use ability"));
	if (abilityHandler->canUseAbility()) {
		playAnimation(useAbillty_animMontag, 1.0f, NAME_None);
		E_playerState = EPlayerState::BLOCK_ANIMATION;
		if (abilityHandler != nullptr) {
			abilityHandler->startCooldown();
		}
	}
}

// Change ability state
void ACharacterBase::changeAbilityState(EAbilityStateEnum newState) {
	if (myAbility.state != newState) {
		myAbility.state = newState;
	}
}

// Change ability current cooldown
void ACharacterBase::changeAbilityCurrentCooldown(float newCooldown) {
	myAbility.currentCooldown = newCooldown;
}

// Spell Section
void ACharacterBase::KeyPress(FKey key) {

	if (E_playerState != EPlayerState::CASTING) {
		return;
	}
	FString keyName = key.GetDisplayName().ToString();

	if (keyName.Len() > 1) {
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Key pressed: %s"), *keyName);
	if (spellHandler) {
		E_SpellCastStatus = spellHandler->castSpell(keyName);

		//UE_LOG(LogTemp, Warning, TEXT("Spell Casting Status: %d"), static_cast<int32>(E_SpellCastStatus));

		if (E_SpellCastStatus == ESpellCastingStatus_Enum::CASTING_COMPLETED) {
			E_playerState = EPlayerState::IDLE;
		}
		// Calling function from bp to follow with the spell casting proccess and handle showing it on the screen
		FString ArgumentValue = keyName;

		FString FunctionNameWithArgument = FString::Printf(TEXT("updateCasting %s"), *ArgumentValue);

		FOutputDeviceNull ar;

		Controller->CallFunctionByNameWithArguments(*FunctionNameWithArgument, ar, nullptr, true);
	}
}

void ACharacterBase::startCastingMode() {
	if (E_playerState == EPlayerState::CASTING) {
		
		E_playerState = EPlayerState::IDLE;

		FString ArgumentValue = TEXT("Hello, World!");

		FString FunctionNameWithArgument = FString::Printf(TEXT("cancelCast %s"), *ArgumentValue);

		FOutputDeviceNull ar;

		Controller->CallFunctionByNameWithArguments(*FunctionNameWithArgument, ar, nullptr, true);

		if (spellHandler) {
			spellHandler->spellCastLimitFailed();
		}
		
		return;
	}
	E_playerState = EPlayerState::CASTING;
	//
	
	UWorld* World = nullptr;
	UE_LOG(LogTemp, Error, TEXT("Calling th f"));
	try
	{
		World = GetWorld();
		UE_LOG(LogTemp, Error, TEXT("Found the world mother fucklers"));

		if (AGameModeBase* GameMode = World->GetAuthGameMode())
		{
			if (AHNSGameModeBase* HNSGameMode = Cast<AHNSGameModeBase>(GameMode))
			{
				UE_LOG(LogTemp, Error, TEXT("GAme mode bitches"));
				spellHandler = NewObject<USpellHandler>();
				spellHandler->setOwner(this);
				spellHandler->spellsList = HNSGameMode->getSpellsList();
				if (spellHandler) {
					spellHandler->testSpells();
					// Calling the function to show the widget
					FString ArgumentValue = TEXT("Hello, World!");

					FString FunctionNameWithArgument = FString::Printf(TEXT("showCast_W %s"), *ArgumentValue);

					FOutputDeviceNull ar;

					Controller->CallFunctionByNameWithArguments(*FunctionNameWithArgument, ar, nullptr, true);
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("Spells error"));

				}
				// Successfully casted to AHNSGameModeBase
				// You can now use HNSGameMode to access the functions and variables of AHNSGameModeBase
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failded to get game mode"));
				// Failed to cast to AHNSGameModeBase
				// Handle the error condition
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failded to get game mode 0009"));

			// Failed to get the game mode
			// Handle the error condition
		}
	}
	catch (const std::exception& Exception)
	{
		UE_LOG(LogTemp, Error, TEXT("Exception occurred while accessing the world: %s"), UTF8_TO_TCHAR(Exception.what()));
	}

}

// Returning the value of the spell structure so its accessable by the blueprint
FSpellStruct ACharacterBase::getSpell() {
	if (spellHandler) {
		return spellHandler->getSpell();
	}
	return FSpellStruct();
}

void ACharacterBase::callSpellCastLimitFailed() {
	spellHandler->spellCastLimitFailed();
}

bool ACharacterBase::callSpellPerformedAction() {
	return spellHandler->getSpellPerformedAction();
}

//the function called by repetedly to update the spell power
void ACharacterBase::updateSpell() {
	spellPower += spellPowerUpdateQty;
	if (spellPower >= maxSpellPower) {
		spellPower = maxSpellPower;
	}
	UE_LOG(LogTemp, Error, TEXT("Updating spell power to : %f"), spellPower);
}
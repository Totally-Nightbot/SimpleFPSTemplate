// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Included for struct FInputActionInstance (Enhanced Input)
#include "InputAction.h"

#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UParticleSystem;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// -- Enhanced Input -- //

	/* Holds collection of currently active and available InputActions */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Fire;

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> LandedCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> JumpCameraShake;

public:
	AFPSCharacter();

	virtual void BeginPlay() override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UParticleSystem* MuzzleFlash;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnJumped_Implementation() override;

	// How to set variables in unreal (edit anywehere allows to be edited anywhere, BP ReadWrite allows for the BP to read and write
	// Replicated means it gets replicated)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int points;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Gameplay")
	bool bIsCarringObject = false;
	bool isServer = false;

	UPROPERTY(Replicated)
	class AFlagObjective* objectiveFlag1;
	UPROPERTY(Replicated)
	class AFlagObjectiveP2* objectiveFlag2;

	UPROPERTY(Replicated)
	FVector startpos;
protected:
	
	/** Fires a projectile. */
	void Fire();

	//Ufunctions and properties are how you talk to the editor to tell it what you want

	//To tell a function to be a serverRPC you define UFunciton and put server
	UFUNCTION(Server, Reliable, WithValidation)

	//you put server_ before the function you want to be a RPC 
	void Server_Fire();

	void MoveInput(const FInputActionValue& InputValue);

	void LookInput(const FInputActionValue& InputValue);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

};


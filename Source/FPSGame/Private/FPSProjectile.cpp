// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"

#include "FlagObjective.h"
#include "FlagObjectiveP2.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "FPSCharacter.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//Sets the object to replicate if the object has authority (checks if code is running on server or client) 
	if (HasAuthority())
	{
		bReplicates = true;
	}
}


void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::Explode, 3.0f, false);
}

void AFPSProjectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));

	// Allow BP to trigger additional logic
	BlueprintExplode();

	Destroy();
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics object
	if ((OtherActor) && (OtherActor != this) && (OtherComp) && OtherComp->IsSimulatingPhysics())
	{
		float RandomIntensity = FMath::RandRange(200.0f, 500.0f);

		OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());

		FVector Scale = OtherComp->GetComponentScale();
		Scale *= 0.8f;

		if (Scale.GetMin() < 0.5f)
		{
			OtherActor->Destroy();
		}
		else
		{
			OtherComp->SetWorldScale3D(Scale);
		}

		UMaterialInstanceDynamic* MatInst = OtherComp->CreateDynamicMaterialInstance(0);
		if (MatInst)
		{
			MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		}

		Explode();
	}

	// Effects the character when projectile hits the them
	if ((OtherActor) && (OtherActor != this) && (OtherComp))
	{

		// Checks the other actor is a character
		AFPSCharacter* mcharacter = Cast<AFPSCharacter>(OtherActor);

		if (mcharacter && GetLocalRole() == ROLE_Authority)
		{
			mcharacter->CurrentHealth -= damage;

			if (mcharacter->CurrentHealth <= 0)
			{
				if (mcharacter->isServer)
				{
					mcharacter->objectiveFlag1->SetActive(mcharacter);
				}
				else
				{
					mcharacter->objectiveFlag2->SetActive(mcharacter);
				}

				mcharacter->bIsCarringObject = false;
				mcharacter->SetActorLocation(mcharacter->startpos);
				mcharacter->CurrentHealth = 100;
			}
		}
	}
}
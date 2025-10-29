// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/GameState.h"
#include "FPSCharacter.h"

#include "FlagObjective.h"

// Sets default values
AFlagObjective::AFlagObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Disables the collision on the component
	RootComponent = MeshComp;

	SphereColliderComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereColliderComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // No Collision, but can do overlap events 
	SphereColliderComp->SetCollisionResponseToAllChannels(ECR_Ignore); // Sets to ignoring all channels that overlap with it 
	SphereColliderComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Sets only the pawn to be able to overlap with it 
	SphereColliderComp->SetupAttachment(MeshComp); // Attaches this under the Mesh Comp

	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called when the game starts or when spawned
void AFlagObjective::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StartLocation = GetActorLocation();
	}
	
}

// Called every frame
void AFlagObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		double ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		FVector NewLocation = StartLocation + FVector(0.0f, 0.0f, 20 * cos(ServerTime * 2.5f));
	
		SetActorLocation(NewLocation);
	}

}

void AFlagObjective::NotifyActorBeginOverlap(AActor* OtherActor) // Add setting the object inactive
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// can use this or HasAuthority()
	if (GetLocalRole() == ROLE_Authority) 
	{
		AFPSCharacter* mCharacter = Cast<AFPSCharacter>(OtherActor);
		
		if (mCharacter)
		{
			mCharacter->bIsCarringObject = true;
			this->Destroy();
		}
	
	}
	

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZone.h"

#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "MyPlayerController.h"

// Sets default values
AWinZone::AWinZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true; 

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetBoxExtent(FVector(200.0f));

	RootComponent = BoxComponent;

	BoxComponent->SetHiddenInGame(false); // For Debug use

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWinZone::HandleOverlap);

}

// Called when the game starts or when spawned
void AWinZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AFPSCharacter* mCharacter = Cast<AFPSCharacter>(OtherActor);
		
		if (mCharacter == nullptr)
		{
			return;
		}

		AMyPlayerController* pController = Cast<AMyPlayerController>(OtherActor);

		if (mCharacter->bIsCarringObject) //Checks to see if the player is the server
		{
			// GetAuthGameMode only exists on the server 
			AFPSGameMode* gMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

			if (gMode) // Send an multicast RPC (Sends a message from the server to the client) 
			{
				gMode->EndGame(mCharacter);

			}
		}

	}

}

// Called every frame
void AWinZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


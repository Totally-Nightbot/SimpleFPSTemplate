// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "Components/BoxComponent.h"
#include "GameFramework/GameState.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	double ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	FVector NewLocation = StartLocation + FVector(0.0f, 0.0f, movement * cos(ServerTime * 2.5f));

	SetActorLocation(NewLocation);
}


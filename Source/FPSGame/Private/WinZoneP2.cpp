// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZoneP2.h"
#include "Components/BoxComponent.h"

// Sets default values
AWinZoneP2::AWinZoneP2()
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

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWinZoneP2::HandleOverlap);
}

// Called when the game starts or when spawned
void AWinZoneP2::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinZoneP2::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult)
{
}

// Called every frame
void AWinZoneP2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


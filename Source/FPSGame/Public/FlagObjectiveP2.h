// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagObjectiveP2.generated.h"

class AFPSCharacter;

UCLASS()
class FPSGAME_API AFlagObjectiveP2 : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AFlagObjectiveP2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Mesh
	UPROPERTY(VisibleAnywhere, Category = "Components");
	class UStaticMeshComponent* MeshComp;

	//Collider
	UPROPERTY(VisibleAnywhere, Category = "Components");
	class USphereComponent* SphereColliderComp;

	FVector StartLocation;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetActive(AFPSCharacter* mcharacter);
};
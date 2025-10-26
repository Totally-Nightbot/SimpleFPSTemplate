// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// Net Multicast RPC
	// When this gets called from the server it gets sent to all clients (including the server) 
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastEndGame(APawn* instigatorPawn);

};

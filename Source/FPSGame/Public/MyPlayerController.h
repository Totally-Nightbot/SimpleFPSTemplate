// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "OnlineSessionSettings.h"

#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	// BlueprintImplementableEvent allows it to be used in blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void EndGame(APawn* InstigatorPawn);
};

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "MyGameStateBase.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	
	GameStateClass = AMyGameStateBase::StaticClass();
}

void AFPSGameMode::EndGame(APawn* instigatorPawn)
{
	if (instigatorPawn)
	{
		instigatorPawn->DisableInput(nullptr); // Disable the input for the pawn that ends the game
	}

	AMyGameStateBase* gState = GetGameState<AMyGameStateBase>();

	if (gState) 
	{
		gState->MultiCastEndGame(instigatorPawn);

	}

}



// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "MyPlayerController.h"

void AMyGameStateBase::MultiCastEndGame_Implementation(APawn* instigatorPawn)
{
	// Get all player controllers on the client 

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		//Get the controller 
		AMyPlayerController* pController = Cast<AMyPlayerController>(it->Get());

		// Check if it exists and is local (this is where is checks becuase the server has 
		// all of the pawns with controllers but only gets the local controller on the server)
		if (pController && pController->IsLocalController())
		{
			//BP implementable event (This is so we can make the widget bp)
			pController->EndGame(instigatorPawn);

			APawn* mPawn = pController->GetPawn();

			// Then Disables all the controllers
			if (mPawn)
			{
				mPawn->DisableInput(pController);
			}
		}
	
	}
}

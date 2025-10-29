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

	AMyPlayerController();

	//LogIn
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void Login();
	void OnLoginCompleteDelegate(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	// Host
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	bool HostSession();
	void OnCreateSessionCompleteDelegate(FName InSessionName, bool bWasSuccessful);

	//Find
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void FindSession();
	void OnFindSessionsCompleteDelegate(bool bWasSuccessful);

	//Join
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void JoinSession();
	void OnJoinSessionCompleteDelegate(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//Quit
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void QuitSession();
};

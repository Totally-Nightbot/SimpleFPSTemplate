// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

//For Getting and using Online Subsystem
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

//For creating custom debug log
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

//User Opening a Level
#include "Kismet/GameplayStatics.h"

//Get current player state
#include "GameFramework/PlayerState.h"

//Search_Keywords, _Presence, _Lobbies, TEC.
#include "Online/OnlineSessionNames.h"

//Session Settings
const FName SESSION_NAME = "AIEI_Session";
TSharedPtr <class FOnlineSessionSearch > searchSettings;

//Custom Debug Logging
#define DISPLAY_LOG(fmt, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT(fmt), ##__VA_ARGS__));

AMyPlayerController::AMyPlayerController()
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	UE_LOG(LogTemp, Warning, TEXT("[PlayerController found subsystem %s]"), *subSystem->GetSubsystemName().ToString());

	Login();

}

void AMyPlayerController::Login()
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem)
	{
		IOnlineIdentityPtr identityInterface = subSystem->GetIdentityInterface();

		if (identityInterface.IsValid())
		{
			ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);

			if (localPlayer != NULL)
			{
				//Get Local controller ID
				int controllerID = localPlayer->GetControllerId();

				//Check if local player is logged in
				if (identityInterface->GetLoginStatus(controllerID) != ELoginStatus::LoggedIn)
				{
					//Call LoginDelegate function to login
					identityInterface->AddOnLoginCompleteDelegate_Handle(controllerID,
						FOnLoginCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnLoginCompleteDelegate));
					identityInterface->AutoLogin(controllerID);
				}

			}
		}
	}
}

void AMyPlayerController::OnLoginCompleteDelegate(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem)
	{
		IOnlineIdentityPtr identityInterface = subSystem->GetIdentityInterface();

		if (identityInterface.IsValid())
		{
			ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);

			if (localPlayer != NULL)
			{
				//Include PlayerState.h
				//Get Uniquenetid to player state
				//The id used by the network to uniquely identify a player
				FUniqueNetIdRepl uniqueNetId = PlayerState->GetUniqueId();
				uniqueNetId.SetUniqueNetId(FUniqueNetIdWrapper(UserId).GetUniqueNetId());
				PlayerState->SetUniqueId(uniqueNetId);

				//Logout login status
				int controllerId = localPlayer->GetControllerId();
				ELoginStatus::Type status = identityInterface->GetLoginStatus(controllerId);
				DISPLAY_LOG("Login Status: %s", ELoginStatus::ToString(status));
			}
		}
		else
		{
			DISPLAY_LOG(" LOGIN FAILED ");
		}
	}
}

bool AMyPlayerController::HostSession()
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem) {
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			TSharedPtr<class FOnlineSessionSettings> sessionSettings = MakeShareable(new FOnlineSessionSettings());

			sessionSettings->NumPublicConnections = 4; // 4 Online Connection
			sessionSettings->NumPrivateConnections = 4; // 4 LAN connections
			sessionSettings->bShouldAdvertise = true; // should broadcast
			sessionSettings->bAllowJoinInProgress = true; // allow join after session begins
			sessionSettings->bAllowInvites = true; // allow to receive invites

			sessionSettings->bUsesPresence = true; // use friendlist
			sessionSettings->bAllowJoinViaPresence = true;// join via friendlist

			sessionSettings->bUseLobbiesIfAvailable = true;// Use lobby if available in subsystem


			// Via Online service and LAN !!
			sessionSettings->Set(SEARCH_KEYWORDS, FString("AIEI_UNREAL_GAME"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			// Add the session create delegate on the Session Interface
			sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this,
				&AMyPlayerController::OnCreateSessionCompleteDelegate));

			TSharedPtr<const FUniqueNetId> uniqueNetId = GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

			bool res = sessionInterface->CreateSession(*uniqueNetId, SESSION_NAME, *sessionSettings);

			if (res)
			{
				DISPLAY_LOG("CREATED SESSION ");
			}
			else
			{
				DISPLAY_LOG("FAILED TO CREATE SESSION ");
				return false;
			}
		}
	}

	return false;
}

void AMyPlayerController::OnCreateSessionCompleteDelegate(FName InSessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Maps/FirstPersonExampleMap")), true, "listen");
	}
}

void AMyPlayerController::FindSession() {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());
	if (subSystem)
	{
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			searchSettings = MakeShareable(new FOnlineSessionSearch());

			searchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			searchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			searchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("AIEI_UNREAL_GAME"), EOnlineComparisonOp::Equals);

			sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this,
				&AMyPlayerController::OnFindSessionsCompleteDelegate));

			// Search for sessions based on search conditions
			TSharedRef<FOnlineSessionSearch> searchSettingsRef = searchSettings.ToSharedRef();
			TSharedPtr<const FUniqueNetId> uniqueNetId = GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

			bool res = sessionInterface->FindSessions(*uniqueNetId, searchSettingsRef);

			if (res)
			{
				DISPLAY_LOG("FOUND SESSION ");
			}

			else
			{
				DISPLAY_LOG("FAILED TO FIND SESSION ");
			}
		}
	}
}

void AMyPlayerController::OnFindSessionsCompleteDelegate(bool bWasSuccessful) {

	if (bWasSuccessful)
	{
		if (searchSettings->SearchResults.Num() == 0)
			DISPLAY_LOG("NO SESSION IDS FOUND !! ");

		DISPLAY_LOG("FOUND SESSION ID: %s", *searchSettings->SearchResults[0].GetSessionIdStr());
		DISPLAY_LOG("HOST NAME : %s", *searchSettings->SearchResults[0].Session.OwningUserName);

		JoinSession();
	}
	else {
		DISPLAY_LOG("FAILED TO FIND SESSION ");
	}
}

void AMyPlayerController::JoinSession()
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem)
	{
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			if (searchSettings->SearchResults[0].IsValid()) {
				sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnJoinSessionCompleteDelegate));

				TSharedPtr<const FUniqueNetId> uniqueNetId = GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

				searchSettings->SearchResults[0].Session.SessionSettings.bUsesPresence = true;
				searchSettings->SearchResults[0].Session.SessionSettings.bUseLobbiesIfAvailable = true;

				sessionInterface->JoinSession(*uniqueNetId, SESSION_NAME, searchSettings->SearchResults[0]);

				DISPLAY_LOG(" JOINING SESSION !! ");
			}

			else
			{
				DISPLAY_LOG(" INVALID SESSION ");
			}
		}
	}
}

void AMyPlayerController::OnJoinSessionCompleteDelegate(FName sessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem)
	{
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{

			if (Result == EOnJoinSessionCompleteResult::Success)
			{

				FString connectionInfo;

				if (sessionInterface->GetResolvedConnectString(SESSION_NAME, connectionInfo))
				{

					UE_LOG_ONLINE_SESSION(Log, TEXT("JOINED SESSION: TRAVELLING TO %s"), *connectionInfo);

					// CLIENT TRAVEL to SERVER
					AMyPlayerController::ClientTravel(connectionInfo, TRAVEL_Absolute);
				}
			}
		}
	}
}

void AMyPlayerController::QuitSession()
{
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem)
	{
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			sessionInterface->DestroySession(SESSION_NAME);

			UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Maps/FirstPersonExampleMap")), true, " ");
		}
	}
}

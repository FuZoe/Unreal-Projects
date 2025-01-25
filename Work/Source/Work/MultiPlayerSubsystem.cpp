// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiPlayerSubsystem.h"
#include <OnlineSessionSettings.h>
#include "OnlineSubsystem.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameSession.h"
#include "Online/OnlineSessionNames.h"
#include "GameFramework/GameUserSettings.h"

void UMultiPlayerSubsystem::Print(FString Message, FColor Color, float Scale, int32 InKey)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(InKey, Scale, Color, Message);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

UMultiPlayerSubsystem::UMultiPlayerSubsystem()
{
}

void UMultiPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetGameScreen(720, 480);
	Print("UMultiPlayerSubsystem::Initialize");
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		FString SubsystemName = Subsystem->GetSubsystemName().ToString();
		Print(SubsystemName);

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			Print("SessionInterface is Valid");
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UMultiPlayerSubsystem::OnCreateSessionComplete);
			
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &UMultiPlayerSubsystem::OnDestroySessionComplete);
			
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UMultiPlayerSubsystem::OnFindSessionComplete);
		}
	}
}

void UMultiPlayerSubsystem::Deinitialize()
{
	Print("UMultiPlayerSubsystem::Deinitialize");
}

void UMultiPlayerSubsystem::CreateServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		Print("ServerName is Empty");
		return;
	}
	FName MySessionName = FName("MultiPlayer Session");
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{
		Print(FString::Printf(TEXT("UMultiPlayerSubsystem::Session with name %s exists"), *MySessionName.ToString()));
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPrivateConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	bool bIsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}
	SessionSettings.bIsLANMatch = bIsLan;

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);

	Print("Server Create Finish");
}

void UMultiPlayerSubsystem::FindServer(FString ServerName)
{
	if (ServerName.IsEmpty())
	{
		Print("ServerName is Empty");
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	bool bIsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}
	SessionSearch->bIsLanQuery = bIsLan;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiPlayerSubsystem::SetGameScreen(float x, float y)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	UserSettings->SetScreenResolution(FIntPoint(x, y));
	UserSettings->SetFullscreenMode(EWindowMode::Windowed);
	UserSettings->ApplySettings(true);
}

void UMultiPlayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful == false)
	{
		Print("UMultiPlayerSubsystem::Create Session Failed");
		return;
	}
	bool res = GetWorld()->ServerTravel("/Game/FirstPerson/Maps/FirstPersonMap?listen");
	Print(FString::Printf(TEXT("ServerTravel: %d"), res));
}

void UMultiPlayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (CreateServerAfterDestroy == true)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiPlayerSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == false)
	{
		Print("UMultiPlayerSubsystem::OnFindSessionComplete Failed");
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	if (Results.Num() > 0)
	{
		Print(FString::Printf(TEXT("Find %d Session"), Results.Num()));
	}
	else
	{
		Print(FString::Printf(TEXT("Session Not Found")));
	}
}

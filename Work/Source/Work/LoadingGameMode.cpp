// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingGameMode.h"

ALoadingGameMode::ALoadingGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter.BP_FirstPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}

void ALoadingGameMode::HostLanGame()
{
	GetWorld()->ServerTravel("/Game/FirstPerson/Maps/FirstPersonMap.FirstPersonMap?Listen");
}

void ALoadingGameMode::JoinLanGame(FString address)
{
	APlayerController* LocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (LocalPlayerController != nullptr)
	{
		LocalPlayerController->ClientTravel(address, TRAVEL_Absolute);
	}
}

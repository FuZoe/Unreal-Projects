// Copyright Epic Games, Inc. All Rights Reserved.

#include "projectqGameMode.h"
#include "projectqCharacter.h"
#include "UObject/ConstructorHelpers.h"

AprojectqGameMode::AprojectqGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "BigProjectGameMode.h"
#include "BigProjectGameState.h"
#include "BigProjectPlayerController.h"
#include "BigProject/Actors/ScoreSquare.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABigProjectGameMode::ABigProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/Blueprints/Framework/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AScoreSquare> ScoreSquareClassFinder(
		TEXT("/Game/Blueprints/BP_ScoreSquare.BP_ScoreSquare_C"));
	if (ScoreSquareClassFinder.Succeeded())
	{
		ScoreSquareClass = ScoreSquareClassFinder.Class;
	}
	AwardPercentage = 0.3f;
	SquareIdx = 0;

	NewPlayerController = nullptr;
}

void ABigProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(GenerateTimeHandle, this, &ABigProjectGameMode::InitTArrayTransform, 1.0f,
	                                       false);
}

void ABigProjectGameMode::OnGameEnd()
{
	for (AScoreSquare* Element : TArrayScoreSquare)
	{
		if (Element)
		{
			Element->Destroy();
		}
	}
	TArrayScoreSquare.Empty();
	GetGameState<ABigProjectGameState>()->OnGameEnd();
}

void ABigProjectGameMode::InitTArrayTransform()
{
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScoreSquare::StaticClass(), OutArray);
	for (auto Actor : OutArray)
	{
		TArrayTransform.Add(Actor->GetTransform());
		Actor->Destroy();
	}
}

void ABigProjectGameMode::InitScoreSquares()
{
	GetWorld()->GetTimerManager().SetTimer(GenerateTimeHandle, this, &ABigProjectGameMode::SpawnScoreSquare, 0.05f,
	                                       true);
}

void ABigProjectGameMode::SpawnScoreSquare()
{
	if (SquareIdx >= TArrayTransform.Num())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "SpawnComplete");
		GetWorld()->GetTimerManager().ClearTimer(GenerateTimeHandle);
		SquareIdx = 0;
		GetWorld()->GetTimerManager().SetTimer(SetSimulateTimerHandle, this, &ABigProjectGameMode::SetSimulate, 0.05f,
		                                       true);
	}
	else
	{
		FTransform SpawnTransform = TArrayTransform[SquareIdx++];

		if (AScoreSquare* ScoreSquare = Cast<AScoreSquare>(GetWorld()->SpawnActor(ScoreSquareClass, &SpawnTransform)))
		{
			float Random = FMath::FRandRange(0.0f, 1.0f);
			if (Random <= AwardPercentage)
			{
				ScoreSquare->SetAwardSquare();
			}
			TArrayScoreSquare.Add(ScoreSquare);
		}
	}
}

void ABigProjectGameMode::SetSimulate()
{
	if (SquareIdx >= TArrayScoreSquare.Num())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "SetSimulateComplete");
		SquareIdx = 0;
		GetGameState<ABigProjectGameState>()->MulticastGameStart();
		GetWorld()->GetTimerManager().ClearTimer(SetSimulateTimerHandle);
	}
	else
	{
		TArrayScoreSquare[SquareIdx++]->Cube->SetSimulatePhysics(true);
	}
}

void ABigProjectGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (HasAuthority())
	{
		NewPlayerController = NewPlayer;
		GetWorld()->GetTimerManager().SetTimer(SetSimulateTimerHandle, this, &ABigProjectGameMode::OnPlayerLogin, 1.0f,
		                                       false);
	}
}

void ABigProjectGameMode::OnPlayerLogin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PlayerLogin");
	if (ABigProjectPlayerController* PlayerController = Cast<ABigProjectPlayerController>(NewPlayerController))
	{
		PlayerController->ClientPostLogin();
		PlayerController->InitUMGPlayerState();
	}
}

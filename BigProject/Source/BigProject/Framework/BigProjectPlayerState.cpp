// Fill out your copyright notice in the Description page of Project Settings.


#include "BigProjectPlayerState.h"
#include "BigProjectCharacter.h"
#include "BigProjectGameState.h"
#include "Net/UnrealNetwork.h"

ABigProjectPlayerState::ABigProjectPlayerState()
{
	ScoreNum = 0;
	bIsReady = false;
	bIsGenerating = false;

	OwnedCharacter = nullptr;
	GameState = nullptr;
}

void ABigProjectPlayerState::InitPlayerData()
{
	for (auto Elem : GameState->PlayerArray)
	{
		ABigProjectPlayerState* PlayerState = Cast<ABigProjectPlayerState>(Elem);
		PlayerState->bIsReady = false;
		PlayerState->ScoreNum = 0;
		if (ABigProjectCharacter* Character = Elem->GetPawn<ABigProjectCharacter>())
		{
			Character->InitCharacterData();
			//Character->SetPlayerReadyState(false);
		}
	}
}

void ABigProjectPlayerState::BeginPlay()
{
	Super::BeginPlay();
	GameState = GetWorld()->GetGameState<ABigProjectGameState>();
	OwnedCharacter = this->GetPawn<ABigProjectCharacter>();
}

void ABigProjectPlayerState::SetCharacterReady(bool IsReady)
{
	bIsReady = IsReady;
	if (OwnedCharacter)
	{
		OwnedCharacter->SetPlayerReadyState(IsReady);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState::OwnedCharacter is Invalid");
	}
}

void ABigProjectPlayerState::SetScoreNum(int32 NewScoreNum)
{
	ScoreNum = NewScoreNum;
	if (OwnedCharacter)
	{
		OwnedCharacter->UpdateScoreNum(ScoreNum);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState::OwnedCharacter is Invalid");
	}
}

void ABigProjectPlayerState::SetServerCharacterReady_Implementation(bool IsReady)
{
	bIsReady = IsReady;

	if (GameState)
	{
		GameState->CheckAllReady();
		GameState->MulticastPlayerReady(GetPlayerId());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState::GameState is Invalid");
	}
}

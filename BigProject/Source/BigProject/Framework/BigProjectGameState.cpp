// Fill out your copyright notice in the Description page of Project Settings.


#include "BigProjectGameState.h"
#include "BigProjectGameMode.h"
#include "BigProjectCharacter.h"
#include "BigProject/Tools/UserDataStruct.h"
#include "BigProject/Tools/GameStateEnum.h"
#include "BigProjectPlayerState.h"
#include "BigProject/Widget/SettlementWidget.h"

void ABigProjectGameState::BeginPlay()
{
	Super::BeginPlay();

	GameState = EGameState::Start;
}

void ABigProjectGameState::SortPlayerData()
{
	for (int i = 0; i < TArrayUserData.Num(); i++)
	{
		for (int j = i + 1; j < TArrayUserData.Num(); j++)
		{
			if (TArrayUserData[i]->Score < TArrayUserData[j]->Score)
			{
				Swap(TArrayUserData[i], TArrayUserData[j]);
			}
			if (TArrayUserData[i]->Score == TArrayUserData[j]->Score &&
				TArrayUserData[i]->UserId > TArrayUserData[j]->UserId)
			{
				Swap(TArrayUserData[i], TArrayUserData[j]);
			}
		}
	}
}

void ABigProjectGameState::CheckAllReady()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "CheckAllReady");
	int32 TempReadyPlayerNum = 0;
	for (auto Element : PlayerArray)
	{
		if (ABigProjectPlayerState* PlayerState = Cast<ABigProjectPlayerState>(Element))
		{
			if (!PlayerState->GetIsReady())
			{
				continue;
			}
			else
			{
				TempReadyPlayerNum++;
			}
		}
	}
	FString ReadyInfo = FString::Printf(TEXT("%d/%d"), TempReadyPlayerNum, PlayerArray.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ReadyInfo);
	if (TempReadyPlayerNum == PlayerArray.Num())
	{
		if (ABigProjectGameMode* GameMode = Cast<ABigProjectGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->InitScoreSquares();
			MulticastStartGenerate();
		}
	}
	else
	{
		MulticastReadyInfo(ReadyInfo);
	}
}

void ABigProjectGameState::MulticastReadyInfo_Implementation(const FString& ReadyInfo)
{
	ABigProjectCharacter* PlayerCharacter = Cast<ABigProjectCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (PlayerCharacter)
	{
		PlayerCharacter->UpdateReadyPlayers(ReadyInfo);
	}
}

void ABigProjectGameState::MulticastPlayerReady_Implementation(const int32 PlayerId)
{
	for (auto Element : PlayerArray)
	{
		if (Element->GetPlayerId() == PlayerId)
		{
			if (ABigProjectCharacter* PlayerCharacter = Cast<ABigProjectCharacter>(Element->GetPawn()))
			{
				PlayerCharacter->SetPlayerReadyState(true);
			}
		}
	}
}

void ABigProjectGameState::MulticastGameStart_Implementation()
{
	GameState = EGameState::OnGame;
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		auto Element = PlayerArray[i];
		ABigProjectPlayerState* PlayerState = Cast<ABigProjectPlayerState>(Element);
		UUserDataStruct* UserData = NewObject<UUserDataStruct>();
		UserData->UserId = PlayerState->GetPlayerId();
		UserData->Score = PlayerState->GetScoreNum();
		TArrayUserData.Insert(UserData, i);
	}
	ABigProjectCharacter* PlayerCharacter = Cast<ABigProjectCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (PlayerCharacter)
	{
		PlayerCharacter->OnStartCount();
	}
}

void ABigProjectGameState::MulticastStartGenerate_Implementation()
{
	ABigProjectCharacter* PlayerCharacter = Cast<ABigProjectCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (PlayerCharacter)
	{
		PlayerCharacter->OnStartGenerate();
	}
}

void ABigProjectGameState::OnGameEnd_Implementation()
{
	GameState = EGameState::End;
	SortPlayerData();
	ShowSettlementUI();
	TArrayUserData.Empty();
}

void ABigProjectGameState::ShowSettlementUI()
{
	if (SettlementClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Class Not Found")));
		return;
	}
	SettlementWidget = CreateWidget<USettlementWidget>(GetWorld(), SettlementClass);
	if (SettlementWidget)
	{
		for (const auto UserData : TArrayUserData)
		{
			FString UserId = FString::FromInt(UserData->UserId);
			SettlementWidget->AddItem(UserId, UserData->Score);
		}
		SettlementWidget->AddToViewport();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not Found")));
	}
}

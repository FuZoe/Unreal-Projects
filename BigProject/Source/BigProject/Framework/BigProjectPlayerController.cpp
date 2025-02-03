// Copyright Epic Games, Inc. All Rights Reserved.


#include "BigProjectPlayerController.h"
#include "BigProjectCharacter.h"
#include "BigProjectPlayerState.h"
#include "BigProjectGameState.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameState.h"
#include "BigProject/Tools/GameStateEnum.h"
#include "BigProject/Widget/SettlementWidget.h"
#include "BigProject/Widget/InfoUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"

void ABigProjectPlayerController::DisablePlayerInput()
{
	DisableInput(this);
	if (APawn* pawn = GetPawn())
	{
		pawn->DisableInput(this);
	}
	GetWorld()->GetTimerManager().SetTimer(EnableInputTimerHandle, this,
	                                       &ABigProjectPlayerController::EnablePlayerInput, 2.f, false);
}

void ABigProjectPlayerController::EnablePlayerInput()
{
	EnableInput(this);
	if (APawn* pawn = GetPawn())
	{
		pawn->EnableInput(this);
	}
}

void ABigProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	DisablePlayerInput();
}

void ABigProjectPlayerController::ClientPostLogin_Implementation()
{
	if (IsLocalPlayerController())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "ClientPostLogin");
		if (UInfoUIWidget* InfoUIWidget = CreateWidget<UInfoUIWidget>(this, InfoWidgetClass))
		{
			ABigProjectCharacter* OwnedCharacter = Cast<ABigProjectCharacter>(GetCharacter());
			ABigProjectPlayerState* APlayerState = OwnedCharacter->GetPlayerState<ABigProjectPlayerState>();
			//APlayerState->InitPlayerData();
			OwnedCharacter->MainUIWidget = InfoUIWidget;
			InfoUIWidget->OwnedPlayerState = APlayerState;
			InfoUIWidget->AddToViewport(-1);
		}
	}
}

void ABigProjectPlayerController::InitUMGPlayerState_Implementation()
{
	for (auto Element : GetWorld()->GetGameState()->PlayerArray)
	{
		if (ABigProjectCharacter* character = Cast<ABigProjectCharacter>(Element->GetPawn()))
		{
			character->RefreshUI();
		}
	}
}

void ABigProjectPlayerController::SetPlayerReady()
{
	if (GetWorld()->GetGameState<ABigProjectGameState>()->GameState != EGameState::Start)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "ABigProjectPlayerController::GameState Not Start");
		return;
	}
	if (ABigProjectPlayerState* OwnerPlayerState = GetPlayerState<ABigProjectPlayerState>())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "ABigProjectPlayerController::SetPlayerReady");
		OwnerPlayerState->OwnedCharacter = Cast<ABigProjectCharacter>(GetCharacter());
		OwnerPlayerState->SetCharacterReady(true);
		OwnerPlayerState->SetServerCharacterReady(true);
	}
}

void ABigProjectPlayerController::CloseSettlement()
{
	if (GetWorld()->GetGameState<ABigProjectGameState>()->GameState != EGameState::End)
	{
		return;
	}
	if (ABigProjectGameState* GameState = GetWorld()->GetGameState<ABigProjectGameState>())
	{
		if (GameState->SettlementWidget == nullptr)
		{
			return;
		}
		else
		{
			GameState->SettlementWidget->RemoveFromParent();
			GameState->SettlementWidget = nullptr;
		}
	}
	if (ABigProjectCharacter* character = Cast<ABigProjectCharacter>(GetCharacter()))
	{
		character->RefreshUI();
	}
	if (ABigProjectPlayerState* playerState = Cast<ABigProjectPlayerState>(PlayerState))
	{
		playerState->InitPlayerData();
	}
	GetWorld()->GetGameState<ABigProjectGameState>()->GameState = EGameState::Start;
}

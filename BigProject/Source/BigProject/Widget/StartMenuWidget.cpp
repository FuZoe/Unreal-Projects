// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UStartMenuWidget::OnCreateRoomButtonClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnCreateRoomButtonClick");

	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;

	GetWorld()->ServerTravel("/Game/Levels/FirstPersonMap.FirstPersonMap?listen");
}

void UStartMenuWidget::OnJoinRoomButtonClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnJoinRoomButtonClick");
}

void UStartMenuWidget::OnRefreshButtonClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnRefreshButtonClick");
}

void UStartMenuWidget::OnReturnButtonClick()
{
	//该点击事件在蓝图中实现
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnReturnButtonClick");
}

void UStartMenuWidget::OnQuitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "OnQuitButtonClick");
}

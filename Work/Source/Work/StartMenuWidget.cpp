// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "MultiPlayerSubsystem.h"

bool UStartMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		Subsystem = GetGameInstance()->GetSubsystem<UMultiPlayerSubsystem>();
	}
	return true;
}

void UStartMenuWidget::OnCreateServerButtonClicked()
{
	if (Subsystem == nullptr) return;
	Subsystem->Print("Create Server");
	Subsystem->CreateServer("TestServer");
}

void UStartMenuWidget::OnJoinGameButtonClicked()
{
	if (Subsystem == nullptr) return;
	Subsystem->Print("Join");
	Subsystem->FindServer("TestServer");
}


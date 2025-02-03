// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "OnlineSubsystemUtils/Classes/FindSessionsCallbackProxy.h"
#include "StartMenuWidget.generated.h"

struct FBlueprintSessionResult;
class IOnlineSubsystem;
class UButton;

UCLASS()
class BIGPROJECT_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, blueprintReadWrite, meta = (BindWidget))
	UButton* CreateRoomButton;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite, meta = (BindWidget))
	UButton* JoinRoomButton;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite, meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite, meta = (BindWidget))
	UButton* ReturnButton;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	TArray<FBlueprintSessionResult> Results;
	
	IOnlineSubsystem* Subsystem;

protected:
	UFUNCTION(BlueprintCallable, Category = "ButtonClick")
	void OnCreateRoomButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonClick")
	void OnJoinRoomButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonClick")
	void OnRefreshButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonClick")
	void OnReturnButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonClick")
	void OnQuitButtonClick();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiPlayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class WORK_API UMultiPlayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Print(FString Message, FColor Color = FColor::MakeRandomColor(), float Scale = 2.0f, int32 InKey = -1);

	UMultiPlayerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void CreateServer(FString ServerName);

	void FindServer(FString ServerName);

	UFUNCTION(BlueprintCallable, Category = "Screen")
	void SetGameScreen(float x, float y);
	
protected:
	IOnlineSessionPtr SessionInterface;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionComplete(bool bWasSuccessful);

	bool CreateServerAfterDestroy;

	FString DestroyServerName;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};

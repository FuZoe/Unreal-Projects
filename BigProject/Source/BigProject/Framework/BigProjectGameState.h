// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BigProjectGameState.generated.h"

enum class EGameState;

UCLASS()
class BIGPROJECT_API ABigProjectGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="GameState")
	TArray<class UUserDataStruct*> TArrayUserData;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="GameEnd")
	TSubclassOf<UUserWidget> SettlementClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="GameEnd")
	class USettlementWidget* SettlementWidget;

	UPROPERTY()
	EGameState GameState;

public:
	UFUNCTION(BlueprintCallable, Category = "GameReady")
	void CheckAllReady();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "GameReady")
	void MulticastPlayerReady(const int32 PlayerId);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "GameStart")
	void MulticastGameStart();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "GameStart")
	void MulticastStartGenerate();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "GameEnd")
	void OnGameEnd();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameEnd")
	void SortPlayerData();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "GameReady")
	void MulticastReadyInfo(const FString& ReadyInfo);

	UFUNCTION(BlueprintCallable, Category = "GameEnd")
	void ShowSettlementUI();
};

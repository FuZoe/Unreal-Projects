// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadingGameMode.generated.h"

/**
 * 
 */
UCLASS(minimalapi)
class ALoadingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoadingGameMode();

protected:
	UFUNCTION(BlueprintCallable, Category = "LanGame")
	void HostLanGame();

	UFUNCTION(BlueprintCallable, Category = "LanGame")
	void JoinLanGame(FString address);
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BigProjectGameMode.generated.h"

class AScoreSquare;

UCLASS(minimalapi)
class ABigProjectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABigProjectGameMode();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnGameEnd();

	UFUNCTION()
	void InitScoreSquares();

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<AScoreSquare*> TArrayScoreSquare;

	UPROPERTY(EditDefaultsOnly)
	TArray<FTransform> TArrayTransform;

	UPROPERTY(EditDefaultsOnly)
	FTimerHandle GenerateTimeHandle;

	UPROPERTY(EditDefaultsOnly)
	FTimerHandle SetSimulateTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	FTimerHandle PlayerLoginTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	AController* NewPlayerController;

	UPROPERTY(EditDefaultsOnly)
	int32 SquareIdx;

	UPROPERTY(EditDefaultsOnly)
	float AwardPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AScoreSquare> ScoreSquareClass;

protected:
	UFUNCTION()
	void InitTArrayTransform();
	
	UFUNCTION()
	void SpawnScoreSquare();

	UFUNCTION()
	void SetSimulate();

	UFUNCTION()
	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION()
	void OnPlayerLogin();
};

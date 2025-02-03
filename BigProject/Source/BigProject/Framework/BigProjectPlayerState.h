#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BigProjectPlayerState.generated.h"

UCLASS()
class BIGPROJECT_API ABigProjectPlayerState : public APlayerState
{
	GENERATED_BODY()

	ABigProjectPlayerState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGenerating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABigProjectCharacter* OwnedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABigProjectGameState* GameState;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsReady() { return bIsReady; }

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Server")
	void SetServerCharacterReady(bool IsReady);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void InitPlayerData();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetCharacterReady(bool IsReady);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	int32 GetScoreNum() {return ScoreNum;}
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetScoreNum(int32 NewScoreNum);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReady;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	int32 ScoreNum;
};

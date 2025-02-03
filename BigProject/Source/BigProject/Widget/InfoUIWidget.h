// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoUIWidget.generated.h"

class UTextBlock;
class UImage;
class ABigProjectPlayerState;

UCLASS()
class BIGPROJECT_API UInfoUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ABigProjectPlayerState* OwnedPlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UWidgetAnimation* AOnCharacterHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UWidgetAnimation* AOnBulletHit;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UFUNCTION(BlueprintCallable, Category = "StateImage")
	void OnCharacterHit();

	UFUNCTION(BlueprintCallable, Category = "SightImage")
	void OnBulletHit();

	UFUNCTION(BlueprintCallable, Category = "Countdown")
	void OnStartGenerate();

	UFUNCTION(BlueprintCallable, Category = "BulletNum")
	void OnBulletNumUpdate(const int32 NewBulletNum);

	UFUNCTION(BlueprintCallable, Category = "ScoreNum")
	void OnScoreNumUpdate(const int32 NewScoreNum);

	UFUNCTION(BlueprintCallable, Category = "Countdown")
	void OnCountdownStart();

	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(bool bIsReady);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void OnPlayerReady(const FString& ReadyInfo);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bIsCounting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	bool bIsGenerating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	float CumulatedSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
	int32 CountdownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TextBlock", meta = (BindWidget))
	UTextBlock* ReadyTipTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TextBlock", meta = (BindWidget))
	UTextBlock* CountdownTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TextBlock", meta = (BindWidget))
	UTextBlock* BulletNumTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TextBlock", meta = (BindWidget))
	UTextBlock* ScoreNumTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Image", meta = (BindWidget))
	UImage* StateImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Image", meta = (BindWidget))
	UImage* SightImage;

	FTimerHandle CountdownTimerHandle;

protected:
	UFUNCTION()
	void OnCounting();
};

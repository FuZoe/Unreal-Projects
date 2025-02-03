// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserStateWidget.generated.h"

class UTextBlock;

UCLASS()
class BIGPROJECT_API UUserStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateTextState(const bool bIsReady, const int32 PlayerId);

	UFUNCTION(BlueprintCallable)
	void SetUserIdTextBlock(const FText& UserIdText);

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* UserIdTextBlock;

protected:
	virtual void NativeConstruct() override;
};

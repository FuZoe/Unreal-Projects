// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenuWidget.generated.h"

/**
 * 
 */

class UMultiPlayerSubsystem;

UCLASS()
class WORK_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintReadWrite, Category = "Subsystem")
	TObjectPtr<UMultiPlayerSubsystem> Subsystem;

	UFUNCTION(BlueprintCallable, Category = "BtnClicked")
	void OnCreateServerButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BtnClicked")
	void OnJoinGameButtonClicked();
	
};

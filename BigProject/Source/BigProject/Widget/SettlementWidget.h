// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettlementWidget.generated.h"

class UDynamicEntryBox;

UCLASS()
class BIGPROJECT_API USettlementWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UDynamicEntryBox* Content;

public:
	UFUNCTION()
	void AddItem(const FString& PlayerId, const int32 Score);
};

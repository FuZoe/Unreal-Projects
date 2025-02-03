// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyValueWidgetItem.generated.h"

class UTextBlock;

UCLASS()
class BIGPROJECT_API UKeyValueWidgetItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* KeyTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (BindWidget))
	UTextBlock* ValueTextBlock;

public:
	UFUNCTION()
	void InitItemInfo(const FText& KeyText, const FText& ValueText);
};

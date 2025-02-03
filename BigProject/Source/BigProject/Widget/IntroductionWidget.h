// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IntroductionWidget.generated.h"

class UButton;

UCLASS()
class BIGPROJECT_API UIntroductionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(blueprintReadWrite, meta = (BindWidget))
	UButton* OKButton;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();
};

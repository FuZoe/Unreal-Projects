// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "UserDataStruct.generated.h"

UCLASS()
class BIGPROJECT_API UUserDataStruct : public UUserDefinedStruct
{
	GENERATED_BODY()

public:
	UUserDataStruct(): UserId(0), Score(0)
	{
	}

	UUserDataStruct(int32 userId): UserId(userId), Score(0)
	{
	}

	UPROPERTY(EditAnywhere)
	int32 UserId;

	UPROPERTY(EditAnywhere)
	int32 Score;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "GameStateEnum.generated.h"

/**
 * 
 */
UENUM()
enum class EGameState
{
	Start,
	OnGame,
	End
};
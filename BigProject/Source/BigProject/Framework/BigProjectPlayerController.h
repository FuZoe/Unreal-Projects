// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BigProject/Components/TP_WeaponComponent.h"
#include "GameFramework/PlayerController.h"
#include "BigProjectPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class BIGPROJECT_API ABigProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void ClientPostLogin();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void InitUMGPlayerState();

	UFUNCTION(BlueprintCallable)
	void DisablePlayerInput();

	UFUNCTION(BlueprintCallable)
	void EnablePlayerInput();
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInfoUIWidget> InfoWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle EnableInputTimerHandle;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetPlayerReady();

	UFUNCTION(BlueprintCallable, Category = Input)
	void CloseSettlement();
};

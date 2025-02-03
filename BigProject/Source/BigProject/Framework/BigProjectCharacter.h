// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BigProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInfoUIWidget;
class UWidgetComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABigProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Widget, meta=(AllowPrivateAccess = "true"))
	UWidgetComponent* UserStateWidgetComponent;
	
public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInfoUIWidget* MainUIWidget;

public:
	ABigProjectCharacter();
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	void OnBulletHit();
	
	UFUNCTION(BlueprintCallable)
	void OnStartCount();

	UFUNCTION(BlueprintCallable)
	void UpdateReadyPlayers(const FString& ReadyInfo);

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UFUNCTION(BlueprintCallable)
	void SetPlayerReadyState(bool bIsReady);

	UFUNCTION(BlueprintCallable)
	void InitCharacterData();

	UFUNCTION(BlueprintCallable)
	void OnStartGenerate();

	UFUNCTION(BlueprintCallable)
	bool OnBulletChange(const int32 Diff);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SpawnBullet(const FTransform& SpawnTransform, AActor* BulletOwner);

	UFUNCTION(BlueprintCallable)
	void OnCharacterHit();

	UFUNCTION(BlueprintCallable)
	void UpdateScoreNum(int32 NewScoreNum);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UUserStateWidget> UserStateWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ABigProjectProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 BulletNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTimerHandle InputStateTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* UnFireSound;
	
protected:
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
};


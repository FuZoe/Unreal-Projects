// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreSquare.generated.h"

class ABigProjectPlayerState;

UCLASS()
class BIGPROJECT_API AScoreSquare : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Mesh")
	UStaticMeshComponent* Cube;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Material")
	UMaterial* AwardMaterial;

public:
	// Sets default values for this actor's properties
	AScoreSquare();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void SetAwardSquare();

	UFUNCTION(Server, Reliable)
	void OnSquareHit(AActor* BulletOwner);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	int32 HitScore;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	int32 BreakScore;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	bool bIsScaled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAddScore(const int32 TargetPlayerId, const int32 NewScore);
	
	UFUNCTION(NetMulticast, Reliable)
	void SquareScale();

	UFUNCTION()
	void PrintPlayerScore();
};


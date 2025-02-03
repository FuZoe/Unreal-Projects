// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreSquare.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "BigProject/Tools/UserDataStruct.h"
#include "BigProject/Framework/BigProjectPlayerState.h"
#include "BigProject/Framework/BigProjectGameState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AScoreSquare::AScoreSquare()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(Root);

	Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("'/Game/Resources/Cube.Cube'"));
	if (MeshObject.Succeeded())
	{
		Cube->SetStaticMesh(MeshObject.Object);
	}
	Cube->SetupAttachment(Root);

	// static ConstructorHelpers::FObjectFinder<UMaterial> Material1(
	// 	TEXT("/Script/Engine.Material'/Game/Resources/Normal.Normal'"));
	// if (Material1.Succeeded())
	// {
	// 	NormalMaterial = Material1.Object;
	// 	NormalMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(NormalMaterial, this);
	// }
	//
	// static ConstructorHelpers::FObjectFinder<UMaterial> Material2(
	// 	TEXT("/Script/Engine.Material'/Game/Resources/Award.Award'"));
	// if (Material2.Succeeded())
	// {
	// 	AwardMaterial = Material2.Object;
	// 	AwardMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(AwardMaterial, this);
	// }
	//
	// if (NormalMaterial)
	// {
	// 	NormalMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(NormalMaterial, Cube);
	// }


	//Cube->SetMaterial(0, NormalMaterialInstanceDynamic);
	HitScore = 20;
	BreakScore = 100;

	Cube->SetSimulatePhysics(false);
	bReplicates = true;
	Super::SetReplicateMovement(true);
	Cube->SetIsReplicated(true);
	Cube->SetLinearDamping(0.1f);
	Cube->SetAngularDamping(0.1f);
}

// Called when the game starts or when spawned
void AScoreSquare::BeginPlay()
{
	Super::BeginPlay();
}

void AScoreSquare::SetAwardSquare_Implementation()
{
	HitScore = FMath::CeilToInt32(1.5 * HitScore);
	BreakScore = FMath::CeilToInt32(1.5 * BreakScore);
	
	if (IsValid(AwardMaterial))
	{
		if (UMaterialInstanceDynamic* AwardMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(AwardMaterial, Cube))
		{
			Cube->SetMaterial(0, AwardMaterialInstanceDynamic);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "UMaterialInstanceDynamic is nullptr");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AwardMaterial is nullptr");
	}
}

void AScoreSquare::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScoreSquare, Cube);
}

void AScoreSquare::OnSquareHit_Implementation(AActor* BulletOwner)
{
	if (ACharacter* Character = Cast<ACharacter>(BulletOwner))
	{
		if (ABigProjectPlayerState* PlayerState = Character->GetPlayerState<ABigProjectPlayerState>())
		{
			int32 NewScore = PlayerState->GetScoreNum();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
			//					FString::Printf(TEXT("NewScore = %d"), NewScore));
			if (bIsScaled)
			{
				NewScore += BreakScore;
			}
			else
			{
				NewScore += HitScore;
			}
			MulticastAddScore(PlayerState->GetPlayerId(), NewScore);
			//PrintPlayerScore();
			SquareScale();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AScoreSquare::Character is Invalid");
	}
}

void AScoreSquare::MulticastAddScore_Implementation(const int32 TargetPlayerId, const int32 NewScore)
{
	if (ABigProjectGameState* GameState = GetWorld()->GetGameState<ABigProjectGameState>())
	{
		for (int i = 0; i < GameState->TArrayUserData.Num(); i++)
		{
			if (GameState->TArrayUserData[i]->UserId == TargetPlayerId)
			{
				GameState->TArrayUserData[i]->Score = NewScore;
			}
			if (GameState->PlayerArray[i]->GetPlayerId() == TargetPlayerId)
			{
				Cast<ABigProjectPlayerState>(GameState->PlayerArray[i])->SetScoreNum(NewScore);
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
			//FString::Printf(TEXT("ThisPlayerId = %d :: TargetPlayerId = %d"), GameState->PlayerArray[i]->GetPlayerId(), TargetPlayerId));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AScoreSquare::GameState is Invalid");
	}
}

void AScoreSquare::SquareScale_Implementation()
{
	if (bIsScaled)
	{
		Destroy();
	}
	else
	{
		bIsScaled = true;
		Cube->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
}

void AScoreSquare::PrintPlayerScore()
{
	for (auto Element : GetWorld()->GetGameState()->PlayerArray)
	{
		if (ABigProjectPlayerState* PlayerState = Cast<ABigProjectPlayerState>(Element))
		{
			FString Message = FString::Printf(TEXT("%d::%d"), PlayerState->GetPlayerId(), PlayerState->GetScoreNum());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, *Message);
		}
	}
}

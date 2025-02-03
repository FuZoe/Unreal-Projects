// Copyright Epic Games, Inc. All Rights Reserved.

#include "BigProjectCharacter.h"
#include "BigProjectPlayerController.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "BigProject/Widget/UserStateWidget.h"
#include "BigProject/Widget/InfoUIWidget.h"
#include "BigProject/Actors/BigProjectProjectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABigProjectCharacter

ABigProjectCharacter::ABigProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Set Default CharacterMesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (MeshObject.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshObject.Object);
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(
			TEXT("'/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C'"));
		if (AnimInstanceClass.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->CastShadow = false;
	GetMesh()->bCastDynamicShadow = false;

	// Create a widget component that will be used when switch player ready state
	UserStateWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UserStateWidget"));
	UserStateWidgetComponent->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FClassFinder<UUserStateWidget> WidgetClass(
		TEXT("'/Game/Blueprints/UMG/UMG_UserStateWidget.UMG_UserStateWidget_C'"));
	if (WidgetClass.Class != nullptr)
	{
		UserStateWidgetComponent->SetWidgetClass(WidgetClass.Class);
		UserStateWidgetComponent->SetRelativeLocation(FVector(0, 0, 110));
		UserStateWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		UserStateWidgetComponent->SetDrawSize(FVector2D(100, 40));
	}

	BulletNum = 30;
}

void ABigProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void ABigProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABigProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABigProjectCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void ABigProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABigProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABigProjectCharacter::InitCharacterData()
{
	BulletNum = 30;
	SetPlayerReadyState(false);
}

void ABigProjectCharacter::OnCharacterHit()
{
	if (IsLocallyControlled())
	{
		if (ABigProjectPlayerController* PlayerController = Cast<ABigProjectPlayerController>(GetController()))
		{
			PlayerController->DisablePlayerInput();
			MainUIWidget->OnCharacterHit();
		}
	}
}

void ABigProjectCharacter::UpdateScoreNum(int32 NewScoreNum)
{
	if (MainUIWidget)
	{
		MainUIWidget->OnScoreNumUpdate(NewScoreNum);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MainUIWidget is Invalid"));
	}
}


void ABigProjectCharacter::OnStartCount()
{
	if (MainUIWidget)
	{
		MainUIWidget->OnCountdownStart();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MainUIWidget is Invalid"));
	}
}

void ABigProjectCharacter::OnBulletHit()
{
	if (IsLocallyControlled())
	{
		MainUIWidget->OnBulletHit();
	}
}

bool ABigProjectCharacter::OnBulletChange(const int32 Diff)
{
	if (IsLocallyControlled())
	{
		int32 TepBulletNum = BulletNum + Diff;

		if (TepBulletNum < 0) return false;
		else
		{
			BulletNum = TepBulletNum;
			MainUIWidget->OnBulletNumUpdate(BulletNum);
			return true;
		}
	}
	else return false;
}

void ABigProjectCharacter::SpawnBullet_Implementation(const FTransform& SpawnTransform, AActor* BulletOwner)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = BulletOwner;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams);
}

void ABigProjectCharacter::OnStartGenerate()
{
	if (MainUIWidget)
	{
		MainUIWidget->OnStartGenerate();
	}
}

void ABigProjectCharacter::RefreshUI()
{
	if (MainUIWidget)
	{
		MainUIWidget->RefreshUI();
	}

	SetPlayerReadyState(false);
}

void ABigProjectCharacter::SetPlayerReadyState(bool bIsReady)
{
	if (MainUIWidget)
	{
		MainUIWidget->SetPlayerReady(bIsReady);
	}
	if (UserStateWidgetComponent)
	{
		if (UUserStateWidget* UserStateWidget = Cast<UUserStateWidget>(UserStateWidgetComponent->GetWidget()))
		{
			int32 PlayerId = GetPlayerState()->GetPlayerId();
			UserStateWidget->UpdateTextState(bIsReady, PlayerId);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "UserStateWidgetInvalid");
		}
	}
}

void ABigProjectCharacter::UpdateReadyPlayers(const FString& ReadyInfo)
{
	if (IsLocallyControlled())
	{
		MainUIWidget->OnPlayerReady(ReadyInfo);
	}
}

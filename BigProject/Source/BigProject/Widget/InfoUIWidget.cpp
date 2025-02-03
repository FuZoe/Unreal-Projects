// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoUIWidget.h"
#include "Animation/WidgetAnimation.h"
#include "BigProject/Framework/BigProjectGameMode.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameMode.h"

void UInfoUIWidget::RefreshUI()
{
	if (BulletNumTextBlock)
	{
		BulletNumTextBlock->SetText(FText::FromString("30"));
	}
	if (CountdownTextBlock)
	{
		CountdownTextBlock->SetText(FText::FromString(""));
	}
	CountdownTime = 30;
}

void UInfoUIWidget::OnCharacterHit()
{
	if (AOnCharacterHit)
	{
		PlayAnimation(AOnCharacterHit);
	}
}

void UInfoUIWidget::OnBulletHit()
{
	if (AOnBulletHit)
	{
		PlayAnimation(AOnBulletHit);
	}
}

void UInfoUIWidget::OnStartGenerate()
{
	bIsGenerating = true;
	if (CountdownTextBlock)
	{
		CountdownTextBlock->SetText(FText::FromString(TEXT("生成中")));
	}
}

void UInfoUIWidget::OnCountdownStart()
{
	bIsGenerating = false;
	bIsCounting = true;
	if (CountdownTextBlock)
	{
		CountdownTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), CountdownTime)));
	}
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UInfoUIWidget::OnCounting, 1.0f, true);
}

void UInfoUIWidget::SetPlayerReady(bool bIsReady)
{
	if (bIsReady)
	{
		ReadyTipTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		ReadyTipTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInfoUIWidget::OnCounting()
{
	CountdownTime--;
	CountdownTextBlock->SetText(FText::FromString(FString::FromInt(CountdownTime)));
	if (CountdownTime == 0)
	{
		CountdownTextBlock->SetText(FText::FromString(TEXT("")));
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		bIsCounting = false;

		if (ABigProjectGameMode* GameMode = Cast<ABigProjectGameMode>(GetWorld()->GetAuthGameMode()))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("InfoUI::StartGameEnd"));
			GameMode->OnGameEnd();
		}
	}
}


void UInfoUIWidget::OnPlayerReady(const FString& ReadyInfo)
{
	if (CountdownTextBlock)
	{
		CountdownTextBlock->SetText(FText::FromString(ReadyInfo));
	}
}

void UInfoUIWidget::OnBulletNumUpdate(const int32 NewBulletNum)
{
	if (BulletNumTextBlock)
	{
		BulletNumTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewBulletNum)));
	}
}

void UInfoUIWidget::OnScoreNumUpdate(const int32 NewScoreNum)
{
	if (ScoreNumTextBlock)
	{
		ScoreNumTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewScoreNum)));
	}
}

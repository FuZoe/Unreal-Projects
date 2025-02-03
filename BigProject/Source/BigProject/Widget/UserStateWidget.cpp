// Fill out your copyright notice in the Description page of Project Settings.


#include "UserStateWidget.h"
#include "Components/TextBlock.h"

void UUserStateWidget::UpdateTextState(const bool bIsReady, const int32 PlayerId)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UUserStateWidget::UpdateTextState"));
	UserIdTextBlock->SetText(FText::FromString(FString::FromInt(PlayerId)));
	if (bIsReady)
	{
		UserIdTextBlock->SetColorAndOpacity(FSlateColor(FColor::Green));
	}
	else
	{
		UserIdTextBlock->SetColorAndOpacity(FSlateColor(FColor::Red));
	}
}

void UUserStateWidget::SetUserIdTextBlock(const FText& UserIdText)
{
	if (UserIdTextBlock)
	{
		UserIdTextBlock->SetText(UserIdText);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "UserIdTextBlock is Invalid");
	}
}

void UUserStateWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SettlementWidget.h"
#include "Components/DynamicEntryBox.h"
#include "Item/KeyValueWidgetItem.h"

void USettlementWidget::AddItem(const FString& PlayerId, const int32 Score)
{
	if (UKeyValueWidgetItem* Item = Cast<UKeyValueWidgetItem>(Content->CreateEntry()))
	{
		Item->InitItemInfo(FText::FromString(PlayerId), FText::FromString(FString::FromInt(Score)));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Adding item Failure");
	}
}

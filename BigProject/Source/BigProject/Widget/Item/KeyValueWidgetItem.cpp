// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyValueWidgetItem.h"
#include "Components/TextBlock.h"

void UKeyValueWidgetItem::InitItemInfo(const FText& KeyText, const FText& ValueText)
{
	if (KeyTextBlock)
	{
		KeyTextBlock->SetText(KeyText);
	}
	if (ValueTextBlock)
	{
		ValueTextBlock->SetText(ValueText);
	}
}

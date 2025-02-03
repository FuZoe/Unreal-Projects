// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyButtonWidgetItem.h"
#include "Components/TextBlock.h"

void UKeyButtonWidgetItem::InitItemTextInfo(const FText& KeyText, const FText& ValueText)
{
	KeyTextBlock->SetText(KeyText);
	ValueButtonText->SetText(ValueText);
}

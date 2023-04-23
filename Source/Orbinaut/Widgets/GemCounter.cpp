// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Widgets/GemCounter.h"

void UGemCounter::SetGemCount(int current, int total)
{
	KeepVisible = (current == total);
	LastSetTime = GetWorld()->TimeSeconds;
	GemCountString = FString::Printf(TEXT("%d/%d"), current, total);
	OnGemCountChanged();
}
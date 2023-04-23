// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"

#include "GemCounter.generated.h"

class UCurveFloat;

UCLASS()
class ORBINAUT_API UGemCounter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString GemCountString = "0/5";

	UFUNCTION(BlueprintImplementableEvent)
		void OnGemCountChanged();

	UFUNCTION(BlueprintCallable)
		void SetGemCount(int current, int total);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool KeepVisible = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LastSetTime = 0.0f;
};

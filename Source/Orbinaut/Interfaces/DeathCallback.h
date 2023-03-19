// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeathCallback.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeathCallback : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface which provides a callback for when this actor has died. Requires a Health component.
 */
class ORBINAUT_API IDeathCallback
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Death")
	void OnDied();
};

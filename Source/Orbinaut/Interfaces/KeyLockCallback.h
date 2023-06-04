// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Orbinaut/Components/LockComponent.h"
#include "Orbinaut/Components/KeyComponent.h"
#include "KeyLockCallback.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKeyLockCallback : public UInterface
{
	GENERATED_BODY()
public:
	static TArray<TScriptInterface<class IKeyLockCallback> > GetKeyLockCallbacks(AActor* root);
	static void Trigger(AActor* root, UKeyComponent* key, ULockComponent* lock, bool locked);
};


class ORBINAUT_API IKeyLockCallback
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Callbacks")
		void OnLockStateChanged(UKeyComponent* key, ULockComponent* lock, bool locked);
};

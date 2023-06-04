// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Interfaces/KeyLockCallback.h"
#include "Orbinaut/Helpers/ActorHelpers.h"

// Add default functionality here for any IKeyLockCallback functions that are not pure virtual.

TArray<TScriptInterface<IKeyLockCallback> > UKeyLockCallback::GetKeyLockCallbacks(AActor* root)
{
	return UActorHelpers::FindActorOrComponentInterfacesRecursive<IKeyLockCallback>(UKeyLockCallback::StaticClass(), root);
}

void UKeyLockCallback::Trigger(AActor* root, UKeyComponent* key, ULockComponent* lock, bool locked)
{
	auto callbacks = UKeyLockCallback::GetKeyLockCallbacks(root);
	for (const auto& callback : callbacks)
	{
		callback->Execute_OnLockStateChanged(callback.GetObject(), key, lock, locked);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathCallback.h"

#include "Orbinaut/Helpers/ActorHelpers.h"


TArray<TScriptInterface<IDeathCallback> > UDeathCallback::GetDeathCallbacks(AActor* root)
{
	return UActorHelpers::FindActorOrComponentInterfacesRecursive<IDeathCallback>(UDeathCallback::StaticClass(), root);
}

void UDeathCallback::Trigger(AActor* root)
{
	auto deathCallbacks = UDeathCallback::GetDeathCallbacks(root);
	for (const auto& callback : deathCallbacks)
	{
		callback->Execute_OnDied(callback.GetObject());
	}
}
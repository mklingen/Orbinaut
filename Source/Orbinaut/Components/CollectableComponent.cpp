// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableComponent.h"

#include "Orbinaut/OrbinautGameModeBase.h"
#include "Orbinaut/Helpers/ActorHelpers.h"
#include "Orbinaut/Interfaces/DeathCallback.h"

// Sets default values for this component's properties
UCollectableComponent::UCollectableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UCollectableComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		GameMode = GetOwner()->GetWorld()->GetAuthGameMode<AOrbinautGameModeBase>();
		if (IsLevelGem)
		{
			// Let the game mode know that this gem exists.
			GameMode->NumGemsInLevel++;
		}
	}
}



void UCollectableComponent::Collect()
{
	// Broadcast our custom event handler.
	if (OnCollect.IsBound())
	{
		OnCollect.Broadcast();
	}
	// Let the game mode know that the gem was collected.
	if (GameMode && IsLevelGem)
	{
		GameMode->OnGemCollected();
	}

	auto deathCallbacks = GET_INTERFACES_RECURSIVE(DeathCallback, GetOwner());
	for (auto& deathCallback : deathCallbacks)
	{
		deathCallback->Execute_OnDied(deathCallback->_getUObject());
	}


	if (GetOwner())
	{
		GetOwner()->Destroy();
	}
}
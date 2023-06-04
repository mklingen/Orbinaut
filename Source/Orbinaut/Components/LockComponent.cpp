// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Components/LockComponent.h"
#include "Orbinaut/Components/KeyComponent.h"
#include "Orbinaut/Interfaces/KeyLockCallback.h"
#include "Orbinaut/Helpers/ActorHelpers.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnActorBeginOverlap function to the OnComponentBeginOverlap event.
	if (GetOwner())
	{
		TInlineComponentArray<UPrimitiveComponent*> primitives;
		GetOwner()->GetComponents<UPrimitiveComponent>(primitives, true);
		for (UPrimitiveComponent* prim : primitives)
		{
			prim->OnComponentBeginOverlap.AddDynamic(this, &ULockComponent::OnOverlapBegin);
		}
	}


}

void ULockComponent::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(KeyAnimationHandle);
	}

}
void ULockComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) 
	{
		return;
	}
	UKeyComponent* key = UActorHelpers::FindComponentRecursive<UKeyComponent>(OtherActor);
	if (!key)
	{
		return;
	}
	if (key->KeyId != KeyId)
	{
		return;
	}

	// Already unlocked, no need to change the state.
	if (!IsLocked)
	{
		return;
	}

	// Unlock. We have the key.
	IsLocked = false;
	UKeyLockCallback::Trigger(GetOwner(), key, this, IsLocked);
	UKeyLockCallback::Trigger(OtherActor, key, this, IsLocked);
	OnLockStateChanged.Broadcast(this, key, IsLocked);
	CurrentKey = OtherActor;

	if (ConstrainKeyOnUnLock)
	{
		ConstrainKey();
	}
}



void ULockComponent::ConstrainKey()
{
	if (!CurrentKey)
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(KeyAnimationHandle);
	KeyAnimationHandle = UActorHelpers::SimpleLerpOverTime(CurrentKey, CurrentKey->GetActorTransform(), GetOwner()->GetActorTransform() * ConstraintTransform, 1.0f);
	TArray<UPrimitiveComponent*> primitives = UActorHelpers::FindComponentsRecursive<UPrimitiveComponent>(CurrentKey);
	for (UPrimitiveComponent* prim : primitives) {
		prim->SetSimulatePhysics(false);
	}

}

void ULockComponent::ReleaseKey()
{
	if (!CurrentKey)
	{
		return;
	}
	TArray<UPrimitiveComponent*> primitives = UActorHelpers::FindComponentsRecursive<UPrimitiveComponent>(CurrentKey);
	for (UPrimitiveComponent* prim : primitives) {
		prim->SetSimulatePhysics(true);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathSFXComponent.h"
#include "Orbinaut/Helpers/SFXHelpers.h"

// Sets default values for this component's properties
UDeathSFXComponent::UDeathSFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDeathSFXComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDeathSFXComponent::OnDied_Implementation()
{
	AActor* owner = GetOwner();
	Effects.Play(owner, owner->GetActorLocation(), owner->GetActorRotation(), owner->GetActorScale());
}

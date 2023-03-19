// Fill out your copyright notice in the Description page of Project Settings.

#include "Orbinaut/Components/KillPlayerOnOverlapComponent.h"
#include "Orbinaut/Characters/OrbinautCharacter.h"

// Sets default values for this component's properties
UKillPlayerOnOverlapComponent::UKillPlayerOnOverlapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UKillPlayerOnOverlapComponent::BeginPlay()
{
	Super::BeginPlay();


	// Bind the OnActorBeginOverlap function to the OnComponentBeginOverlap event.
	if (GetOwner())
	{
		GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UKillPlayerOnOverlapComponent::OnActorBeginOverlap);
	}
	
}


void UKillPlayerOnOverlapComponent::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AOrbinautCharacter* character = Cast<AOrbinautCharacter>(OtherActor);
	if (character)
	{
		OnKill.Broadcast();
		character->Die();
	}
}
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
		TArray<UActorComponent*> primtives = GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass());
		for (UActorComponent* component : primtives)
		{
			UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(component);
			if (prim && prim->GetCollisionProfileName() == "KillZone")
			{
				// Lol hack.
				prim->OnComponentBeginOverlap.AddDynamic(this, &UKillPlayerOnOverlapComponent::OnOverlapBegin);
			}
		}
	}
	
}


void UKillPlayerOnOverlapComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
												   AActor* OtherActor, 
												   UPrimitiveComponent* OtherComp, 
												   int32 OtherBodyIndex, 
												   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionProfileName() == "GravitySource")
	{
		// Lol, hack.
		return;
	}
	AOrbinautCharacter* character = Cast<AOrbinautCharacter>(OtherActor);
	if (character)
	{
		OnKill.Broadcast();
		character->Die();
	}
}
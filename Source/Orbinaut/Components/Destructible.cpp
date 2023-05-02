// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Components/Destructible.h"


UDestructable::UDestructable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UDestructable::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDestructable::OnDied_Implementation()
{
	if (StayAliveAfterDeath <= 0.0f)
	{
		GetOwner()->Destroy();
	}
	else
	{
		FTimerHandle timer;
		FTimerDelegate d;
		d.BindLambda([this]()
			{
				GetOwner()->Destroy();
			});
		GetWorld()->GetTimerManager().SetTimer(timer, d, StayAliveAfterDeath, false);
	}
}


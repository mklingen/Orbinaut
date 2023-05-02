// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Orbinaut/Interfaces/DeathCallback.h"
#include "Destructible.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API UDestructable : public UActorComponent, public IDeathCallback
{
	GENERATED_BODY()

public:	
	UDestructable();

	// Amount of time to stay alive after the object is destroyed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Destructable")
		float StayAliveAfterDeath = 0.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnDied_Implementation();
};

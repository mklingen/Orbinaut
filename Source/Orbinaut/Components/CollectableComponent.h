// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API UCollectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollectableComponent();

	UFUNCTION(BlueprintCallable)
		void Collect();

	// Called when the player collects this collectable component.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnCollect OnCollect;

	// If true, contributes to gem collection in the level, which opens
	// the next portal.
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool IsLevelGem = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Pointer to the main game mode.
	UPROPERTY()
		class AOrbinautGameModeBase* GameMode = nullptr;

};

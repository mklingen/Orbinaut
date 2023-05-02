// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillPlayerOnOverlapComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKill);


// Kills the player when any collider overlaps with the player's collider.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API UKillPlayerOnOverlapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKillPlayerOnOverlapComponent();


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
		TArray<FName> IgnoreCollisionProfiles;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the player overlaps this component.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnKill OnKill;

};

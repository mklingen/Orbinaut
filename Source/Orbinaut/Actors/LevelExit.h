// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelExit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerOverlap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClosed);

UCLASS()
class ORBINAUT_API ALevelExit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelExit();

	// Called when something begins overlapping this actor-.
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Portal")
		bool IsOpen = false;

	// Delay upon the player entering the portal before the next level is triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Portal")
		float DelayForNextLevel = 1.0f;

	UFUNCTION(BlueprintCallable)
		void Open();

	UFUNCTION(BlueprintCallable)
		void Close();

	// Called when the player overlaps this actor, and the thing is open.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPlayerOverlap OnPlayerOverlap;

	// Called when opened.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPlayerOverlap OnOpened;

	// Called when closed.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPlayerOverlap OnClosed;

	// The name of the next level. If empty, reloads the current level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Portal", meta = (GetOptions = "GetAllMapNames"))
		FName NextLevelName = "";


	UFUNCTION(CallInEditor)
		TArray<FName> GetAllMapNames();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollider;
	
	// Pointer to the main game mode.
	UPROPERTY()
		class AOrbinautGameModeBase* GameMode = nullptr;
};

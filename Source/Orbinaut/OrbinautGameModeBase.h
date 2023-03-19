// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbinautGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPortalOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPortalClose);

UCLASS()
class ORBINAUT_API AOrbinautGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Number of gems in the current level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gems")
		int32 NumGemsInLevel = 0;
	
	// Number of gems that have been collected.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gems")
		int32 NumCollectedGems = 0;

	// Return whether we can open a portal to the next level.
	UFUNCTION(BlueprintCallable)
		bool CanOpenPortal() const;

	// Called whenever a gem gets collected.
	UFUNCTION(BlueprintCallable)
		void OnGemCollected();

	// Called whenever a new level is loaded.
	UFUNCTION(BlueprintCallable)
		void BeforeNewLevelLoaded();

	// Called whenever the level is exited.
	UFUNCTION(BlueprintCallable)
		void ExitLevel(float DelayTime);

	// Called to restart the current level.
	UFUNCTION(BlueprintCallable)
		void RestartLevel(float DelayTime);


	// Called when the portal is opened.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPortalOpen OnOpenPortal;

	// Called when the portal is closed.
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPortalClose OnClosePortal;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};

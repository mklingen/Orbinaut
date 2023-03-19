// Copyright Epic Games, Inc. All Rights Reserved.


#include "OrbinautGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Orbinaut/Helpers/PrintHelper.h"

bool AOrbinautGameModeBase::CanOpenPortal() const
{
	return NumCollectedGems >= NumGemsInLevel;
}


void AOrbinautGameModeBase::OnGemCollected()
{
	NumCollectedGems++;
	if (CanOpenPortal())
	{
		LOGI("Level complete! All gems collected!");
		OnOpenPortal.Broadcast();
	}
	else
	{
		OnClosePortal.Broadcast();
	}
}

void AOrbinautGameModeBase::BeforeNewLevelLoaded()
{
	NumCollectedGems = 0;
	NumGemsInLevel = 0;
}

void AOrbinautGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOrbinautGameModeBase::ExitLevel(float DelayTime)
{
	// TODO load next level
	RestartLevel(DelayTime);
}


void AOrbinautGameModeBase::RestartLevel(float DelayTime)
{
	auto restart = [this]()
	{
		BeforeNewLevelLoaded();
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	};
	if (DelayTime <= 0.0)
	{
		restart();
	}
	else
	{
		FTimerHandle delay;
		GetWorldTimerManager().SetTimer(delay, restart, DelayTime, false);
	}
}
// Copyright Epic Games, Inc. All Rights Reserved.


#include "OrbinautGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Orbinaut/Helpers/PrintHelper.h"
#include "Orbinaut/Widgets/GemCounter.h"
#include "Blueprint/UserWidget.h"

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
	UpdateGemCount();
}

void AOrbinautGameModeBase::UpdateGemCount()
{
	if (GemCounterWidget)
	{
		GemCounterWidget->SetGemCount(NumCollectedGems, NumGemsInLevel);
	}
}

void AOrbinautGameModeBase::BeforeNewLevelLoaded()
{
	NumCollectedGems = 0;
	NumGemsInLevel = 0;
	UpdateGemCount();
}

void AOrbinautGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (GemCounterClass)
	{
		GemCounterWidget = CreateWidget<UGemCounter>(GetWorld(), GemCounterClass);
		if (GemCounterWidget)
		{
			GemCounterWidget->AddToViewport();
		}
	}
	UpdateGemCount();
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
		UpdateGemCount();
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
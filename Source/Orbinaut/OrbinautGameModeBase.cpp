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

void AOrbinautGameModeBase::ExitLevel(float DelayTime, const FName& NextLevel)
{
	RestartLevel(DelayTime, NextLevel);
}


void AOrbinautGameModeBase::RestartLevel(float DelayTime, const FName& NextLevel)
{
	auto restart = [this, NextLevel]()
	{
		BeforeNewLevelLoaded();
		FName next = NextLevel;
		if (0 == next.GetStringLength() || NextLevel.IsNone())
		{
			next = FName(*GetWorld()->GetName());
		}
		UGameplayStatics::OpenLevel(this, next, false);
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
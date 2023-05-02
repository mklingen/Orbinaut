// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Actors/CameraBounds.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACameraBounds::ACameraBounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Transform"));
	if (WITH_EDITOR)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
	}
	DebugDraw = true;
}

// Called when the game starts or when spawned
void ACameraBounds::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	DebugDraw = false;
	
}

void ACameraBounds::Tick(float DeltaTime)
{
	if (WITH_EDITOR && DebugDraw)
	{
		DrawDebugBox
		(
			GetWorld(),
			Bounds.GetCenter() + GetActorLocation(),
			Bounds.GetExtent(),
			GetActorRotation().Quaternion(),
			FColor::Cyan,
			false,
			0,
			0,
			4
		);
	}
}

bool ACameraBounds::ShouldTickIfViewportsOnly() const
{
	return true;
}
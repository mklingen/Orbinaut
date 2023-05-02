// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Actors/LevelExit.h"
#include "Orbinaut/Characters/OrbinautCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/ObjectLibrary.h"
#include "Orbinaut/OrbinautGameModeBase.h"

// Sets default values
ALevelExit::ALevelExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	RootComponent = SphereCollider;
	SphereCollider->InitSphereRadius(150.0f);
	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OnOverlapBegin);
	GameMode = GetWorld()->GetAuthGameMode<AOrbinautGameModeBase>();

	if (GameMode)
	{
		GameMode->OnOpenPortal.AddDynamic(this, &ALevelExit::Open);
		GameMode->OnClosePortal.AddDynamic(this, &ALevelExit::Close);
	}
}

void ALevelExit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOpen)
	{
		return;
	}

	if (OtherActor->IsA<AOrbinautCharacter>())
	{
		AOrbinautCharacter* character = Cast<AOrbinautCharacter>(OtherActor);
		if (character && character->IsThePlayer)
		{
			// Prevent the player from moving, getting controls, etc.
			character->Disable(false);
			OnPlayerOverlap.Broadcast();
			if (GameMode)
			{
				GameMode->ExitLevel(DelayForNextLevel, NextLevelName);
			}
		}
	}
}


void ALevelExit::Open()
{
	IsOpen = true;
	OnOpened.Broadcast();
}


void ALevelExit::Close()
{
	IsOpen = false;
	OnClosed.Broadcast();
}

TArray<FName> ALevelExit::GetAllMapNames()
{
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Levels"));
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	UE_LOG(LogTemp, Warning, TEXT("Found maps: %d"), AssetDatas.Num());

	TArray<FName> Names = TArray<FName>();

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];

		auto name = AssetData.AssetName;
		Names.Add(name);
	}
	return Names;
}

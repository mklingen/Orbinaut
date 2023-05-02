// Fill out your copyright notice in the Description page of Project Settings.


#include "Orbinaut/Components/KeepCameraInBounds.h"
#include "Orbinaut/Actors/CameraBounds.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKeepCameraInBounds::UKeepCameraInBounds()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UKeepCameraInBounds::BeginPlay()
{
	Super::BeginPlay();
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector newPos = player->GetActorLocation() + OffsetFromPlayer;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraBounds::StaticClass(), FoundActors);

	if (!FoundActors.IsEmpty())
	{
		Bounds = Cast<ACameraBounds>(FoundActors[0]);
	}
	if (Bounds)
	{
		const FBox& box = Bounds->Bounds;
		FVector posInBounds = Bounds->GetActorTransform().InverseTransformPosition(newPos);
		if (!box.IsInsideOrOn(posInBounds))
		{
			newPos = Bounds->GetActorTransform().TransformPosition(box.GetClosestPointTo(posInBounds));
		}
	}
	GetOwner()->SetActorRotation(RotationInWorld);
	GetOwner()->SetActorLocation(newPos);
	
}


// Called every frame
void UKeepCameraInBounds::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector newPos = player->GetActorLocation() + OffsetFromPlayer;
	FVector currPos = GetOwner()->GetActorLocation();
	GetOwner()->SetActorRotation(RotationInWorld);
	if (Bounds)
	{
		const FBox& box = Bounds->Bounds;
		FVector posInBounds = Bounds->GetActorTransform().InverseTransformPosition(newPos);
		if (!box.IsInsideOrOn(posInBounds))
		{
			newPos = Bounds->GetActorTransform().TransformPosition(box.GetClosestPointTo(posInBounds));
		}
	}
	GetOwner()->SetActorLocation(FMath::Lerp(currPos, newPos, DeltaTime * MoveSpeed));
}


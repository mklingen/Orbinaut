// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Box.h"
#include "ComponentVisualizer.h"
#include "CameraBounds.generated.h"

UCLASS()
class ORBINAUT_API ACameraBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraBounds();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounds")
		FBox Bounds;

	virtual bool ShouldTickIfViewportsOnly() const override;
	void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawing")
		bool DebugDraw = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};

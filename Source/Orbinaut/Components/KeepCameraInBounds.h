// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeepCameraInBounds.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API UKeepCameraInBounds : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeepCameraInBounds();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
		FVector OffsetFromPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
		FRotator RotationInWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
		float MoveSpeed = 10;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		class ACameraBounds* Bounds = nullptr;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

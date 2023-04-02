// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RadialPhysicsBody.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API URadialPhysicsBody : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URadialPhysicsBody();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector CalculateGravity(const FVector& center, float constant, float linear, float square);

	UPROPERTY()
		TArray<class URadialPhysicsSource*> SourcesInRadius;

	UPROPERTY()
		TArray<class UNiagaraComponent*> AttractionParticleSystems;

	FVector SetFromClosestSource();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SFX")
		class UNiagaraSystem* AttractionParticles = nullptr;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool DebugDraw = false;

	// Called when we enter the radius of some physics source.
	UFUNCTION()
		void OnEnterRadius(class URadialPhysicsSource* source);

	// Called when we leave the radius of some physics source.
	UFUNCTION()
		void OnLeaveRadius(URadialPhysicsSource* source);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsDead = false;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RadialPhysicsSource.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API URadialPhysicsSource : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URadialPhysicsSource();

	UFUNCTION()
		void OnSphereOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void InitializeComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Constant = 1.0e4f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Linear = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Square = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float InfluenceRadius = 5000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsEnabled = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SFX")
		class UNiagaraSystem* AttractionParticles = nullptr;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollider;
};

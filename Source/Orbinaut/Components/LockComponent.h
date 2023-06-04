// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Orbinaut/Components/KeyComponent.h"
#include "LockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLockStateChanged, ULockComponent*, Lock, UKeyComponent*, Key, bool, IsLocked);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBINAUT_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Key that goes into this lock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName KeyId;
	
	// Whether this lock is currently locked.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsLocked = true;

	// If true, the key will be constrained to the given constraint transform on lock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool ConstrainKeyOnUnLock = true;
	
	// Pose to constrain the key in w.r.t the lock when it goes in.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FTransform ConstraintTransform;

	UFUNCTION(BlueprintCallable)
		void ConstrainKey();
	
	UFUNCTION(BlueprintCallable)
		void ReleaseKey();


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnLockStateChanged OnLockStateChanged;

private:
	UPROPERTY()
		AActor* CurrentKey = nullptr;
};

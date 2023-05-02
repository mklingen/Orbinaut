// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "OrbinautCharacter.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

// Direction of the thruster.
UENUM(BlueprintType)
enum class EThrustDirection : uint8 {
	VE_Up          UMETA(DisplayName = "Up"),
	VE_Down        UMETA(DisplayName = "Down"),
	VE_Left        UMETA(DisplayName = "Left"),
	VE_Right	   UMETA(DisplayName = "Right")
};

// Sucking and blowing state.
UENUM(BlueprintType)
enum class ETractorBeamMode : uint8 {
	Off          UMETA(DisplayName = "Off"),
	Sucking        UMETA(DisplayName = "Sucking"),
	Blowing        UMETA(DisplayName = "Blowing")
};

// Represents a thruster and its special effects.
USTRUCT(BlueprintType)
struct FThrusterSFX {
	GENERATED_BODY()
public:
	// Direction that the thruster is in.
	UPROPERTY()
		EThrustDirection ThrustDirection;
	// Particle system to spawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditInline = true))
		class UNiagaraComponent* ParticleComponent = nullptr;
};

UCLASS()
class ORBINAUT_API AOrbinautCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOrbinautCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle the current input controls for thrust and turning.
	void HandleThrustControls(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void SetLeftRight(float Value);

	UFUNCTION()
		void SetUpDown(float Value);

	UFUNCTION()
		void StartSucking();

	UFUNCTION()
		void StartBlowing();

	UFUNCTION()
		void StopTractorBeam();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tractor Beam")
		class UNiagaraComponent* TractorBeamParticles = nullptr;

	// -1 = suck, +1 = blow, 0 = stop.
	UFUNCTION()
		void SetTractorBeamValue(float value);

	UFUNCTION(BlueprintCallable)
		void Die();

	// The current Left/Right input. -1 for left, +1 for right.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		float LeftRight = 0;

	// The current Thrust input. -1 for negative (braking) thrust, +1 for positive thrust.
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		float UpDown = 0;

	// The maximum thrust force in the forward direction.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		float ThrustForce = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		bool DebugDraw = false;

	// If true, this character is the player.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool IsThePlayer = true;

	// If true, this character is the player.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool IsDead = false;

	// Time to delay after the player dies.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		float DelayRestartAfterDeath = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tractor Beam")
		float MaxSuckForce = 40000;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tractor Beam")
		float MaxBlowForce = 40000;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
		void Disable(bool bHide);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class URadialPhysicsBody* RadialPhysics;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class URadialPhysicsSource* TractorBeam;

	UPROPERTY(EditDefaultsOnly, Category = "Thrusters")
		TMap<EThrustDirection, FThrusterSFX> Thrusters;

	// Pointer to the main game mode.
	UPROPERTY()
		class AOrbinautGameModeBase* GameMode = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tractor Beam")
		TEnumAsByte<ETractorBeamMode> TractorBeamMode = ETractorBeamMode::Off;

	void UpdateTractorBeamParticles(float direcction, bool active);

	void FireThruster(EThrustDirection direction, bool bOn);
};

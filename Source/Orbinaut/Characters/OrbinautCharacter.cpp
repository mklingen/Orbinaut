// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbinautCharacter.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Orbinaut/Components/RadialPhysicsBody.h"
#include "NiagaraComponent.h"
#include "Orbinaut/Components/CollectableComponent.h"
#include "Orbinaut/OrbinautGameModeBase.h"
#include "Orbinaut/Helpers/ActorHelpers.h"
#include "Orbinaut/Interfaces/DeathCallback.h"

// Sets default values
AOrbinautCharacter::AOrbinautCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Set up the sphere collider
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));

    // Set up the physics rigid body
    if (SphereCollider)
    {
        SetRootComponent(SphereCollider);
        SphereCollider->InitSphereRadius(150.f);
        SphereCollider->SetCollisionProfileName(TEXT("Pawn"));
        SphereCollider->SetSimulatePhysics(true);
        SphereCollider->SetEnableGravity(false);
        SphereCollider->SetMassOverrideInKg(NAME_None, 100.f, true);
        SphereCollider->SetGenerateOverlapEvents(true);
        SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AOrbinautCharacter::BeginOverlap);
    }

    // Set up the static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    if (StaticMesh)
    {
        StaticMesh->SetSimulatePhysics(false);
        StaticMesh->SetEnableGravity(false);
    }
    StaticMesh->SetupAttachment(RootComponent);

    RadialPhysics = CreateDefaultSubobject<URadialPhysicsBody>(TEXT("RadialPhysics"));
    UEnum* staticEnum = StaticEnum < EThrustDirection>();
    Thrusters.Empty();
    // Initialize each thruster.
    for (const EThrustDirection& key : { EThrustDirection::VE_Down, EThrustDirection::VE_Left, EThrustDirection::VE_Right, EThrustDirection::VE_Up }) {
        FThrusterSFX thruster;
        thruster.ThrustDirection = key;
        FString name = staticEnum->GetNameStringByValue(static_cast<int64_t>(key));
        thruster.ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(FName(name + "_Particles"));
        thruster.ParticleComponent->SetupAttachment(RootComponent);
        RemoveOwnedComponent(thruster.ParticleComponent);
        thruster.ParticleComponent->CreationMethod = EComponentCreationMethod::Instance;
        AddOwnedComponent(thruster.ParticleComponent);
        Thrusters.Add(key, thruster);
    }
}

// Called when the game starts or when spawned
void AOrbinautCharacter::BeginPlay()
{
	Super::BeginPlay();
    GameMode = GetWorld()->GetAuthGameMode<AOrbinautGameModeBase>();
}

// Called every frame
void AOrbinautCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    HandleThrustControls(DeltaTime);

}

void AOrbinautCharacter::HandleThrustControls(float DeltaTime)
{
    if (IsDead)
    {
        return;
    }

    // If any thrust.
    if (FMath::Abs(LeftRight) > 1e-4 || FMath::Abs(UpDown) > 1e-4)
    {
        const FTransform& tf = GetTransform();
        // Get world vectors for up/down left right.
        FVector thrustVecLeftRight = tf.GetRotation().RotateVector(FVector3d::RightVector) * LeftRight;
        FVector thrustVecUpDown = tf.GetRotation().RotateVector(FVector3d::ForwardVector) * UpDown;
        // Get the total thrust, and normalize it, applying the maximum value of updown/leftright
        // to its length.
        FVector totalThrust = (thrustVecLeftRight + thrustVecUpDown);
        float maxThrust = FMath::Max(FMath::Abs(LeftRight), FMath::Abs(UpDown));
        totalThrust.Normalize();
        totalThrust *= maxThrust;
        // Multiply by the thrust force and delta time to give us a force vector.
        totalThrust *= ThrustForce * DeltaTime;
        if (DebugDraw)
        {
            DrawDebugLine(GetWorld(), tf.GetLocation(), tf.GetLocation() + totalThrust, FColor::Green);
        }
        SphereCollider->AddForce(totalThrust, NAME_None, true /* accel change */);
    }
    
    constexpr double threshold = 1.0e-1;
    FireThruster(EThrustDirection::VE_Down, UpDown > threshold);
    FireThruster(EThrustDirection::VE_Up, UpDown < -threshold);
    FireThruster(EThrustDirection::VE_Left, LeftRight > threshold);
    FireThruster(EThrustDirection::VE_Right, LeftRight < -threshold);

}

void AOrbinautCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind the LeftRight axis to a function that sets the current value of the LeftRight member variable
    PlayerInputComponent->BindAxis("LeftRight", this, &AOrbinautCharacter::SetLeftRight);

    // Bind the Thrust axis to a function that sets the current value of the Thrust member variable
    PlayerInputComponent->BindAxis("UpDown", this, &AOrbinautCharacter::SetUpDown);
}

void AOrbinautCharacter::FireThruster(EThrustDirection direction, bool bOn)
{
    if (!Thrusters.Contains(direction))
    {
        return;
    }
    UNiagaraComponent* component = Thrusters[direction].ParticleComponent;
    if (!component)
    {
        return;
    }
    if (bOn)
    {
        component->Activate();
    }
    else
    {
        component->Deactivate();
    }
}

void AOrbinautCharacter::SetLeftRight(float Value)
{
    LeftRight = Value;
}

void AOrbinautCharacter::SetUpDown(float Value)
{
    UpDown = Value;
}

void AOrbinautCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (IsDead)
    {
        return;
    }

    UCollectableComponent* collectable = OtherActor->FindComponentByClass<UCollectableComponent>();
    if (collectable)
    {
        collectable->Collect();
    }
}
void AOrbinautCharacter::Disable(bool bHide)
{
    if (bHide)
    {
        SetHidden(true);
    }
    IsDead = true;
    if (RadialPhysics)
    {
        RadialPhysics->IsDead = true;
    }
    if (StaticMesh)
    {
        StaticMesh->SetSimulatePhysics(false);
        if (bHide)
        {
            StaticMesh->SetVisibility(false, true);
        }
    }
    if (SphereCollider)
    {
        SphereCollider->SetSimulatePhysics(false);
    }
    FireThruster(EThrustDirection::VE_Down, false);
    FireThruster(EThrustDirection::VE_Up, false);
    FireThruster(EThrustDirection::VE_Left, false);
    FireThruster(EThrustDirection::VE_Right, false);
}

void AOrbinautCharacter::Die()
{
    if (IsDead)
    {
        return;
    }
    Disable(true);

    auto deathCallbacks = GET_INTERFACES_RECURSIVE(DeathCallback, this);
    for (auto& deathCallback : deathCallbacks)
    {
        deathCallback->Execute_OnDied(deathCallback->_getUObject());
    }

    if (GameMode)
    {
        GameMode->RestartLevel(DelayRestartAfterDeath);
    }
}

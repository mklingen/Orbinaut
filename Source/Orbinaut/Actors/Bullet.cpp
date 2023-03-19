#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "Orbinaut/Characters/OrbinautCharacter.h"
#include "Orbinaut/Components/DeathSFXComponent.h"
#include "Orbinaut/Helpers/ActorHelpers.h"
#include "Orbinaut/Interfaces/DeathCallback.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    ParticleTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleTrail"));
    ParticleTrail->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = RootComponent;

    // Set default values
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    MeshComponent->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);

    DeathSFX = CreateDefaultSubobject<UDeathSFXComponent>(TEXT("Death SFX"));
}

void ABullet::BeginPlay()
{
    Super::BeginPlay();

    // Set homing behavior
    if (bIsHoming)
    {
        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player != nullptr)
        {
            ProjectileMovement->HomingTargetComponent = Player->GetRootComponent();
            ProjectileMovement->HomingAccelerationMagnitude = 10000.0f;
        }
    }

    SetLifeSpan(BulletLifeTime);
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor != GetOwner())
    {
        AOrbinautCharacter* character = Cast<AOrbinautCharacter>(OtherActor);
        if (character)
        {
            OnPlayerHit(OtherActor);
            character->Die();
        }
        else
        {
            DestroyBullet();
        }
    }
}

void ABullet::OnPlayerHit(AActor* OtherActor)
{
    DestroyBullet();
}

void ABullet::DestroyBullet()
{
    ProjectileMovement->Deactivate();
    ParticleTrail->Deactivate();
    MeshComponent->SetVisibility(false);
    SetLifeSpan(0.1f);
    auto deathCallbacks = GET_INTERFACES_RECURSIVE(DeathCallback, this);
    for (auto& deathCallback : deathCallbacks)
    {
        deathCallback->Execute_OnDied(deathCallback->_getUObject());
    }
}
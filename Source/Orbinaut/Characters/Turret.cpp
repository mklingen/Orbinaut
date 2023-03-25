#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"

ATurret::ATurret()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
    RootComponent = BaseMeshComponent;

    GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMeshComponent"));
    GunMeshComponent->SetupAttachment(BaseMeshComponent);
}

void ATurret::BeginPlay()
{
    Super::BeginPlay();

    // Start firing bullets
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::FireBullet, FireRate, true);
}

void ATurret::FireBullet()
{
    AActor* Player = GetPlayer();
    if (Player != nullptr)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector TurretLocation = GetActorLocation();
        float DistanceToPlayer = FVector::Distance(PlayerLocation, TurretLocation);

        if (DistanceToPlayer <= FireRange)
        {
            RotateTurretTowardsPlayer();
            // Fire bullet at player
            FRotator Rotation = (PlayerLocation - TurretLocation).Rotation();
            Rotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
            FVector Position = GunMeshComponent->GetComponentTransform().TransformPosition(MuzzleLocation);
            ShootSFX.Play(GetOwner(), Position, Rotation, GunMeshComponent->GetComponentScale());
            Position.Z = 0;
            AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletClass, Position, Rotation);
            if (Bullet != nullptr)
            {
                Bullet->SetOwner(this);
            }
        }
    }
}

void ATurret::RotateTurretTowardsPlayer()
{
    AActor* Player = GetPlayer();
    if (Player != nullptr)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector TurretLocation = GetActorLocation();
        FRotator Rotation = (PlayerLocation - TurretLocation).Rotation();
        GunMeshComponent->SetWorldRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));
    }
}

AActor* ATurret::GetPlayer()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController != nullptr)
    {
        return PlayerController->GetPawn();
    }
    return nullptr;
}
 void ATurret::Tick(float DeltaTime)
{
     AActor* Player = GetPlayer();
     if (Player != nullptr)
     {
         FVector PlayerLocation = Player->GetActorLocation();
         FVector TurretLocation = GetActorLocation();
         float DistanceToPlayer = FVector::Distance(PlayerLocation, TurretLocation);

         if (DistanceToPlayer <= FireRange)
         {
             RotateTurretTowardsPlayer();
         }
     }
}
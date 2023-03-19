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
            AActor* Bullet = GetWorld()->SpawnActor<AActor>(BulletClass, GunMeshComponent->GetComponentLocation(), Rotation);
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
        BaseMeshComponent->SetWorldRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));
        GunMeshComponent->SetWorldRotation(Rotation);
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
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Orbinaut/Helpers/SFXHelpers.h"
#include "Turret.generated.h"

UENUM()
enum ETurretShootMode
{
    TrackPlayer     UMETA(DisplayName = "Track Player"),
    Fixed      UMETA(DisplayName = "Fixed")
};

UCLASS()
class ORBINAUT_API ATurret : public APawn
{
    GENERATED_BODY()

public:
    ATurret();

    // Called every frame.
    virtual void Tick(float DeltaTime) override;


protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* BaseMeshComponent;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* GunMeshComponent;

    // How often the turret will shoot, per second.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        float FireRate = 1.0f;

    // Range of the turret. It will only shoot when in range.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        float FireRange = 5000.0f;

    // The kind of bullet to create.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        TSubclassOf<AActor> BulletClass;

    // SFX to create whenever we shoot.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        FSFXHelpers ShootSFX;

    // Location of the muzzle expressed with respect to the gun mesh. Bullets are
    // spawned from here, and SFX are spawned here too.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        FVector MuzzleLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shooting")
        TEnumAsByte<ETurretShootMode> ShootMode = ETurretShootMode::TrackPlayer;
    
private:
    UFUNCTION()
        void FireBulletAtPlayer();

    UFUNCTION()
        void FireBullet(const FRotator& Rotation);

    UFUNCTION()
        void FireTimerCallback();


    UFUNCTION()
        void RotateTurretTowardsPlayer();

    AActor* GetPlayer();

    FTimerHandle FireTimerHandle;
};
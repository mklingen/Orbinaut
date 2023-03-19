#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UCLASS()
class ORBINAUT_API ATurret : public APawn
{
    GENERATED_BODY()

public:
    ATurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* BaseMeshComponent;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* GunMeshComponent;

    UPROPERTY(EditAnywhere)
        float FireRate = 1.0f;

    UPROPERTY(EditAnywhere)
        float FireRange = 5000.0f;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> BulletClass;

private:
    UFUNCTION()
        void FireBullet();

    UFUNCTION()
        void RotateTurretTowardsPlayer();

    AActor* GetPlayer();

    FTimerHandle FireTimerHandle;
};
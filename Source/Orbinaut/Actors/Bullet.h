#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class ORBINAUT_API ABullet : public AActor
{
    GENERATED_BODY()

public:
    ABullet();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere)
        UNiagaraComponent* ParticleTrail;

    UPROPERTY(EditAnywhere)
        UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere)
        class UDeathSFXComponent* DeathSFX;

    UFUNCTION()
        void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
        void OnPlayerHit(AActor* OtherActor);

    UPROPERTY(EditAnywhere)
        float BulletLifeTime = 5.0f;

    UPROPERTY(EditAnywhere)
        bool bIsHoming = false;

private:
    UFUNCTION()
        void DestroyBullet();
};
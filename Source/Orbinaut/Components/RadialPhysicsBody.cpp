// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialPhysicsBody.h"

#include "Orbinaut/Components/RadialPhysicsSource.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
URadialPhysicsBody::URadialPhysicsBody()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryComponentTick.TickInterval = 1.0f / 60.0;

}


// Called when the game starts
void URadialPhysicsBody::BeginPlay()
{
	Super::BeginPlay();

	
}


FVector URadialPhysicsBody::CalculateGravity(const FVector& center, float constant, float linear, float square)
{
	FVector pos = this->GetOwner()->GetActorLocation();
	FVector diff = center - pos;
	diff.Z = 0;
	float diffLen = FMath::Max(diff.Length(), 1e-3);
	FVector dir = diff.GetSafeNormal();
	return dir * constant + dir * (1.0 / diffLen) * linear + dir * (1.0 / (diffLen * diffLen)) * square;
}

// Called every frame
void URadialPhysicsBody::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDead)
	{
		return;
	}
	UPrimitiveComponent* prim = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (prim && prim->IsSimulatingPhysics())
	{
		FVector grav = SetFromClosestSource();

		if (grav.SquaredLength() > 1e-5)
		{
			prim->AddForce(grav * DeltaTime, NAME_None, true /* accel change */);

			if (DebugDraw)
			{
				auto pos = GetOwner()->GetActorLocation();
				DrawDebugLine(GetWorld(), pos, pos + grav * DeltaTime, FColor::Yellow);
			}
		}
	}
	UProjectileMovementComponent* movement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (movement)
	{
		FVector grav = SetFromClosestSource();

		if (grav.SquaredLength() > 1e-5)
		{
			movement->AddForce(grav * DeltaTime);
		}
	}
}



void URadialPhysicsBody::OnEnterRadius(class URadialPhysicsSource* source)
{
	if (!SourcesInRadius.Contains(source))
	{
		SourcesInRadius.Add(source);
	}
	if (AttractionParticles)
	{
		AttractionParticleSystems.Add(UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetOwner(), AttractionParticles,
			source->GetComponentLocation(), FRotator(), FVector::One(), true, true));
	}
}


void URadialPhysicsBody::OnLeaveRadius(URadialPhysicsSource* source)
{
	if (SourcesInRadius.Contains(source))
	{
		int idx = -1;
		for (int i = 0; i < SourcesInRadius.Num(); i++)
		{
			if (SourcesInRadius[i] == source)
			{
				idx = i;
				break;
			}
		}
		SourcesInRadius.Remove(source);
		if (idx >= 0 && AttractionParticles)
		{
			AttractionParticleSystems[idx]->DestroyComponent();
			AttractionParticleSystems.RemoveAt(idx);
		}
	}
}

FVector URadialPhysicsBody::SetFromClosestSource()
{
	FVector grav = FVector::Zero();
	FVector pos = GetOwner()->GetActorLocation();
	const URadialPhysicsSource* closest = nullptr;
	float closestDist = std::numeric_limits<float>::max();
	int k = 0;
	static const FString start("Start");
	static const FString end("End");
	static const FString intensity("Intensity");
	for (const URadialPhysicsSource* source : SourcesInRadius)
	{
		FVector g = CalculateGravity(source->GetComponentLocation(), source->Constant, source->Linear, source->Square);
		if (AttractionParticles)
		{
			UNiagaraComponent* particles = AttractionParticleSystems[k];
			particles->SetNiagaraVariableVec3(start, source->GetComponentLocation());
			particles->SetNiagaraVariableVec3(end, GetOwner()->GetActorLocation());
			particles->SetNiagaraVariableFloat(intensity, g.Length());
			particles->Activate();
		}
		grav += g;
		k++;
	}
	return grav;
}
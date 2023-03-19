// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialPhysicsBody.h"

#include "Orbinaut/Components/RadialPhysicsSource.h"


// Sets default values for this component's properties
URadialPhysicsBody::URadialPhysicsBody()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URadialPhysicsBody::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
	if (prim)
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
}



void URadialPhysicsBody::OnEnterRadius(class URadialPhysicsSource* source)
{
	if (!SourcesInRadius.Contains(source))
	{
		SourcesInRadius.Add(source);
	}
}


void URadialPhysicsBody::OnLeaveRadius(URadialPhysicsSource* source)
{
	if (SourcesInRadius.Contains(source))
	{
		SourcesInRadius.Remove(source);
	}
}

FVector URadialPhysicsBody::SetFromClosestSource()
{
	FVector grav = FVector::Zero();
	FVector pos = GetOwner()->GetActorLocation();
	const URadialPhysicsSource* closest = nullptr;
	float closestDist = std::numeric_limits<float>::max();
	for (const URadialPhysicsSource* source : SourcesInRadius)
	{
		grav += CalculateGravity(source->GetComponentLocation(), source->Constant, source->Linear, source->Square);
	}
	return grav;
}
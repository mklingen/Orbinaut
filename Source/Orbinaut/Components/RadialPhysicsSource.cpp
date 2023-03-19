// Fill out your copyright notice in the Description page of Project Settings.
#include "RadialPhysicsSource.h"

#include "Components/SphereComponent.h"
#include "Orbinaut/Components/RadialPhysicsBody.h"


// Sets default values for this component's properties
URadialPhysicsSource::URadialPhysicsSource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

}

void URadialPhysicsSource::InitializeComponent()
{
	// First try to find an existing collider, if it exists.
	if (GetOwner())
	{
		SphereCollider = GetOwner()->FindComponentByClass<USphereComponent>();
	}
	if (SphereCollider)
	{
		return;
	}

	// If it doesn't, create one instead.
	SphereCollider = NewObject<USphereComponent>(GetOwner(), TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetOwner()->GetRootComponent());
	SphereCollider->CreationMethod = EComponentCreationMethod::Instance;
	SphereCollider->RegisterComponent();
	SphereCollider->InitSphereRadius(InfluenceRadius);
	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


// Called when the game starts
void URadialPhysicsSource::BeginPlay()
{
	Super::BeginPlay();

	// No owner, so can't do anything.
	if (!GetOwner())
	{
		return;
	}


	if (SphereCollider)
	{
		SphereCollider->SetSphereRadius(0.001f);
		SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &URadialPhysicsSource::OnSphereOverlap);
		SphereCollider->OnComponentEndOverlap.AddDynamic(this, &URadialPhysicsSource::OnSphereEndOverlap);

		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([this]()
		{
				// Forces us to overlap with the player, lol
				SphereCollider->SetSphereRadius(InfluenceRadius);
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 0.1f, false);
	}
	
}

void URadialPhysicsSource::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, 
										   AActor* OtherActor, 
										   UPrimitiveComponent* OtherComp, 
										   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	URadialPhysicsBody* body = OtherActor->FindComponentByClass<URadialPhysicsBody>();
	if (body)
	{
		body->OnEnterRadius(this);
	}
}


void URadialPhysicsSource::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	URadialPhysicsBody* body = OtherActor->FindComponentByClass<URadialPhysicsBody>();
	if (body)
	{
		body->OnLeaveRadius(this);
	}
}


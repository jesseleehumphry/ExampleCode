// Copyright JLH 2020

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Comp_Grabber.h"

#define OUT

UComp_Grabber::UComp_Grabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UComp_Grabber::BeginPlay()
{
	Super::BeginPlay();
	SetupInputBinding();
}

// Called every frame
void UComp_Grabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Handle)
	{
		if (Handle->GrabbedComponent)
		{
			GetPlayerViewRotation();
			Handle->SetTargetLocation(LineTraceEnd);
		}
	}
	// if physics handle is attach, move
	
}

void UComp_Grabber::Grab ()
{
	FHitResult HitResult = TraceForPhysicsObject();
	AActor* HitActor = HitResult.GetActor();
	
	if (HitActor)
	{
		UPrimitiveComponent* CompToGrab = HitResult.GetComponent();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *HitActor->GetName());
		GetPlayerViewRotation();
		Handle = FindPhysicsHandle(HitActor);
		if (Handle)
		{
			Handle->GrabComponentAtLocation(
				CompToGrab,
				NAME_None,
				LineTraceEnd
			);	
		}
	}
}

void UComp_Grabber::Release ()
{
	if (Handle)
	{
		Handle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UComp_Grabber::FindPhysicsHandle(AActor* HitActor) const
{
	UPhysicsHandleComponent* PhysicsHandle = HitActor->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle not present on %s"), *HitActor->GetName());
		return nullptr;
	}

	return PhysicsHandle;
}

void UComp_Grabber::SetupInputBinding()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("UseKey", EInputEvent::IE_Pressed, this, &UComp_Grabber::Grab);
		InputComponent->BindAction("UseKey", EInputEvent::IE_Released, this, &UComp_Grabber::Release);	
	}
}

FHitResult UComp_Grabber::TraceForPhysicsObject()
{
	GetPlayerViewRotation();

	FHitResult TraceHitOut;
	const FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
        OUT TraceHitOut,
        ViewPointVector,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

	return TraceHitOut;
}

void UComp_Grabber::GetPlayerViewRotation()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint (
        OUT ViewPointVector,
        OUT ViewPointRotator
    );

	ViewXVector = ViewPointRotator.Vector();
	LineTraceEnd = ViewPointVector + FVector(ViewXVector * ReachDistance);
}

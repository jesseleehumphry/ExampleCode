// Copyright JLH 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Vector.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Comp_Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UComp_Grabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UComp_Grabber();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	FVector ViewXVector;
	FVector LineTraceEnd;
	FVector ViewPointVector;
	FRotator ViewPointRotator;

	UPROPERTY()
	UPhysicsHandleComponent* Handle = nullptr;
	
	UPROPERTY(EditAnywhere, Category="Customization")
	float ReachDistance = 100.f;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void SetupInputBinding();
	void GetPlayerViewRotation();

	FHitResult TraceForPhysicsObject();
	UPhysicsHandleComponent* FindPhysicsHandle(AActor* HitActor) const;

};

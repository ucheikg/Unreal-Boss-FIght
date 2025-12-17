// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Creed.generated.h"


UCLASS()
class MYPROJECT_API ACreed : public ACharacter
{
	GENERATED_BODY()
	// defining components of the blueprint
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* lGlove;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Lorigin;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* rGlove;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Rorigin;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))

	UStaticMeshComponent* lcontact;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))

	UStaticMeshComponent* rcontact;

public:
	// Sets default values for this character's properties
	ACreed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// defining a function in public so it can be called by other scripts
	void damaged();


protected:
	// casting values but should not be used by other scripts
	float lAlpha;
	bool lIsLerping;
	bool lIsReturning;
	bool rIsLerping;
	bool rIsReturning;
	float rAlpha;
	// creating the player camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	// creating  function to be used
	void MoveFoward(float InputValue);
	void MoveRight(float InputValue);

	void TurnCamera(float InputVlaue);
	void LookUp(float InputValue);

	void leftHook();
	void rightHook();
	
	// casting values but should not be used by other scripts
	float lLerpSpeed = 1.9f;
	float rLerpSpeed = 1.9f;
	float travelDistance = 1000.0f;
	float delay;
	float health;
	float power;
	float radius;
	// creating locations so they can be used
	FVector lStartLocation;
	FVector rStartLocation;
	FVector targetLocation;

	FVector startPoint;
	FVector endPoint;

	FVector rStart;
	FVector lStart;
	
	FVector rEnd;
	FVector lEnd;
	// creating the overlap function
	UFUNCTION() void OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tyson_Character.generated.h"

UCLASS()
class MYPROJECT_API ATyson_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* lGlove;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Lorigin;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* rGlove;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Rorigin;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Range;

public:
	// Sets default values for this character's properties
	ATyson_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void leftHook(); 
	void rightHook();
	void moveTo(float DeltaTime);
	float timeOut;
	bool inRange;

protected:


	float lAlpha;
	bool lIsLerping;
	bool lIsReturning;
	bool rIsLerping;
	bool rIsReturning;
	float rAlpha;

	float lLerpSpeed = 1.9f;
	float rLerpSpeed = 1.9f;
	float travelDistance = 1000.0f;
	float health;
	float power;
	float radius;
	float dRadius;

	FVector lStartLocation;
	FVector rStartLocation;
	FVector targetLocation;

	FVector startPoint;
	FVector endPoint;
	FVector longPoint;

	FVector rStart;
	FVector lStart;
	FVector rangeStart;

	FVector rEnd;
	FVector lEnd;
	FVector rangeEnd;
};

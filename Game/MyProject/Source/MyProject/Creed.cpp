// Fill out your copyright notice in the Description page of Project Settings.


#include "Creed.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

// Sets default values
ACreed::ACreed()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Glove = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Glove->SetupAttachment(RootComponent);
	alpha = 0.0f;
	isLerping = false;
	isReturning = false;

}

// Called when the game starts or when spawned
void ACreed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector start = Camera->GetComponentLocation();
	FVector end = start + (Camera->GetForwardVector() * 1000);

	FHitResult HitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, collisionParams);
	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 2.0f);

	targetLocation = end;

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
	}

	if (isLerping) {
		alpha += DeltaTime * lerpSpeed;
		alpha = FMath::Clamp(alpha, 0.0f, 1.0f);

		FVector newLocation;
		if (!isReturning) {
			newLocation = FMath::Lerp(startLocation, targetLocation, alpha);
		}
		else {
			newLocation = FMath::Lerp(targetLocation, startLocation, alpha);
		}
		Glove->SetWorldLocation(newLocation);

		if (alpha >= 0.1f) {
			if (!isReturning) {
				isReturning = true;
				alpha = 0.0f;
			}
			else {
				isLerping = false;
				isReturning = false;
				alpha = 0.0f;
			}
		}

	}

}

// Called to bind functionality to input
void ACreed::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACreed::Jump);
	PlayerInputComponent->BindAction("leftHook", IE_Pressed, this, &ACreed::leftHook);
	
	PlayerInputComponent->BindAxis("MoveFoward", this, &ACreed::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACreed::MoveRight);

	PlayerInputComponent->BindAxis("TurnCamera", this, &ACreed::TurnCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &ACreed::LookUp);
}

void ACreed::MoveFoward(float InputValue)
{

	FVector FowardDirection = GetActorForwardVector();
	AddMovementInput(FowardDirection, InputValue);

}

void ACreed::MoveRight(float InputValue)
{

	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputValue);

}

void ACreed::TurnCamera(float InputVlaue)
{

	AddControllerYawInput(InputVlaue);

}

void ACreed::LookUp(float InputValue)
{

	AddControllerPitchInput(InputValue);

}

void ACreed::leftHook()
{

	if (isLerping) return;
	startLocation = Glove->GetComponentLocation();
	FVector gloveFoward = Glove->GetForwardVector();
	targetLocation = startLocation + gloveFoward * travelDistance;

	alpha = 0.0f;
	isLerping = true;
	isReturning = false;
}



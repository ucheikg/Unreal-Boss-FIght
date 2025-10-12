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
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Gloves = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gloves"));
	Gloves->SetupAttachment(Root);

	Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest"));
	Chest->SetupAttachment(Root);

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
	
	FVector start = Chest->GetComponentLocation();
	FVector end = start + (Chest->GetForwardVector() * 1000);

	FHitResult HitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, collisionParams);
	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 2.0f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
	}

}

// Called to bind functionality to input
void ACreed::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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



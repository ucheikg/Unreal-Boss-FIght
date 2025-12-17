// Fill out your copyright notice in the Description page of Project Settings.


#include "Creed.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Tyson_Character.h"

// Sets default values
ACreed::ACreed()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//setting up a attachment for all object in the blueprint
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	//tells the camera to use the rotation of the character
	Camera->bUsePawnControlRotation = true;
	//setting up a attachment for all object in the blueprint
	lGlove = CreateDefaultSubobject<UStaticMeshComponent>("lGlove");
	lGlove->SetupAttachment(RootComponent);
	rGlove = CreateDefaultSubobject<UStaticMeshComponent>("rGlove");
	rGlove->SetupAttachment(RootComponent);

	//setting up a attachment for all object in the blueprint
	Lorigin = CreateDefaultSubobject<UStaticMeshComponent>("Loriginmesh");
	Lorigin->SetupAttachment(RootComponent);
	Rorigin = CreateDefaultSubobject<UStaticMeshComponent>("Rorigin");
	Rorigin->SetupAttachment(RootComponent);
	// valuse which will be used later on
	lAlpha = 0.0f;
	lIsLerping = false;
	lIsReturning = false;

	rAlpha = 0.0f;
	rIsLerping = false;
	rIsReturning = false;

	health = 100.0f;
	power = 5.0f;

	radius = 80.0f;
}

// Called when the game starts or when spawned
void ACreed::BeginPlay()
{
	Super::BeginPlay();
	//if the gloves overlap with a object call the function OnOverlapStart
	lGlove->OnComponentBeginOverlap.AddDynamic(this, &ACreed::OnOverlapStart);
	rGlove->OnComponentBeginOverlap.AddDynamic(this, &ACreed::OnOverlapStart);
	
}

// Called every frame
void ACreed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//start and end values for the raycast
	startPoint = Camera->GetComponentLocation();
	endPoint = startPoint + (Camera->GetForwardVector() * 1000);
	//setting parameters for the raycast
	FHitResult HitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	//draws raycast with the values provided
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startPoint, endPoint, ECC_Visibility, collisionParams);
	
	
	// sets the target location for the gloves and the gloves origin
	targetLocation = endPoint;
	lStartLocation = Lorigin->GetComponentLocation();
	rStartLocation = Rorigin->GetComponentLocation();
	// checks if the right hand needs to move
	if (rIsLerping) 
	{
		// the rate at which alpha increases value
		rAlpha += DeltaTime * rLerpSpeed;
		//clamps alpha between 0.0f, 1.0f;
		rAlpha = FMath::Clamp(rAlpha, 0.0f, 1.0f);
		// definig the right gloves new location
		FVector rnewLocation;
		// if it is not returning then the glove should move to target location within the duration of alpha
		if (!rIsReturning) {
			rnewLocation = FMath::Lerp(rStartLocation, targetLocation, rAlpha);
		}
		// if it is returning then the glove should move from the target location back to its origin
		else {
			rnewLocation = FMath::Lerp(targetLocation, rStartLocation, rAlpha);
		}

		//cast the gloves location
		rGlove->SetWorldLocation(rnewLocation);

		// make sure alpha  has completed its increase to 1
		if (rAlpha >= 1.0f) {
			// once its complete it should check if its already returning if not it should start and reset alpha to 0
			if (!rIsReturning) {
				rIsReturning = true;
				rAlpha = 0.0f;
			}
			// if it is returning or has already returned it should reset all values
			else
			{
				rIsLerping = false;
				rIsReturning = false;
				rAlpha = 0.0f;
			}
		}
	}
	// follows the exact logic of the right glove just applied to the left glove
	if (lIsLerping) {
		
		lAlpha += DeltaTime * lLerpSpeed;
		lAlpha = FMath::Clamp(lAlpha, 0.0f, 1.0f);

		FVector lnewLocation;
		if (!lIsReturning) {
			lnewLocation = FMath::Lerp(lStartLocation, targetLocation, lAlpha);
		}
		else {
			lnewLocation = FMath::Lerp(targetLocation, lStartLocation, lAlpha);
		}


		lGlove->SetWorldLocation(lnewLocation);
		
		if (GEngine)
		{
			float DeltaTime = GetWorld()->DeltaTimeSeconds;	
		}

		if (lAlpha >= 1.0f) {
			
			if (!lIsReturning) {
				lIsReturning = true;
				lAlpha = 0.0f;
			}
			else
			{
				lIsLerping = false;
				lIsReturning = false;
				lAlpha = 0.0f;
			}
			
		}

	}
	// if health is equal to zero destroy the player
	if (health <= 0) {
		this->Destroy();
	}

}

// Called to bind functionality to input
void ACreed::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// binds the players inputs to functions using Unreal's input system
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACreed::Jump);
	PlayerInputComponent->BindAction("leftHook", IE_Pressed, this, &ACreed::leftHook);
	PlayerInputComponent->BindAction("rightHook", IE_Pressed, this, &ACreed::rightHook);
	
	PlayerInputComponent->BindAxis("MoveFoward", this, &ACreed::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACreed::MoveRight);

	PlayerInputComponent->BindAxis("TurnCamera", this, &ACreed::TurnCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &ACreed::LookUp);
}

// tells the object to move foward or backwards based of what the input value is
void ACreed::MoveFoward(float InputValue)
{

	FVector FowardDirection = GetActorForwardVector();
	AddMovementInput(FowardDirection, InputValue);

}
// tells object to move left or right based of input value
void ACreed::MoveRight(float InputValue)
{

	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputValue);

}

// tells the camera how to move based of input

void ACreed::TurnCamera(float InputVlaue)
{

	AddControllerYawInput(InputVlaue);

}

void ACreed::LookUp(float InputValue)
{

	AddControllerPitchInput(InputValue);

}

// triggers the left hook attack
void ACreed::leftHook()
{
	// stops it from repeating the lerp midway if the are multiple inputs
	if (lIsLerping) return;
	// resets alpha and begins lerp
	lAlpha = 0.0f;
	lIsLerping = true;
	lIsReturning = false;
}

// triggers the right hook attack
void ACreed::rightHook() 
{
	// stops it from repeating the lerp midway if the are multiple inputs
	if (rIsLerping) return;
	// resets alpha and begins lerp
	rAlpha = 0.0f;
	rIsLerping = true;
	rIsReturning = false;
}

// sets parameter for overlap
void ACreed::OnOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// cast the boss to a placeholder and also makes it the other actor
	ATyson_Character* boss = Cast<ATyson_Character>(OtherActor);
	// boss is the other actor calls damage function in boss
	if (boss != nullptr) {
		boss->damaged();
	}


}
// function that decreases player health when hit
void ACreed::damaged()
{
	health--;
}



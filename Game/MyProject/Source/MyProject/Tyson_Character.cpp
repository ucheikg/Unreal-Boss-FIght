// Fill out your copyright notice in the Description page of Project Settings.


#include "Tyson_Character.h"

// Sets default values
ATyson_Character::ATyson_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	lGlove = CreateDefaultSubobject<UStaticMeshComponent>("lGlove");
	lGlove->SetupAttachment(RootComponent);
	rGlove = CreateDefaultSubobject<UStaticMeshComponent>("rGlove");
	rGlove->SetupAttachment(RootComponent);


	Lorigin = CreateDefaultSubobject<UStaticMeshComponent>("Loriginmesh");
	Lorigin->SetupAttachment(RootComponent);
	Rorigin = CreateDefaultSubobject<UStaticMeshComponent>("Rorigin");
	Rorigin->SetupAttachment(RootComponent);

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
void ATyson_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATyson_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	startPoint = GetActorLocation();
	endPoint = startPoint + (GetActorForwardVector() * 1000);

	rStart = rGlove->GetComponentLocation();

	lStart = lGlove->GetComponentLocation();

	lEnd = lStart + (lGlove->GetForwardVector().RotateAngleAxis(90, FVector::UpVector) * radius);
	rEnd = rStart + (rGlove->GetForwardVector() * radius);

	FHitResult HitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, startPoint, endPoint, ECC_Visibility, collisionParams);
	DrawDebugLine(GetWorld(), startPoint, endPoint, FColor::Green, false, 2.0f);

	targetLocation = endPoint;
	lStartLocation = Lorigin->GetComponentLocation();
	rStartLocation = Rorigin->GetComponentLocation();

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
	}

	if (rIsLerping)
	{
		rAlpha += DeltaTime * rLerpSpeed;
		rAlpha = FMath::Clamp(rAlpha, 0.0f, 1.0f);

		FVector rnewLocation;

		if (!rIsReturning) {
			rnewLocation = FMath::Lerp(rStartLocation, targetLocation, rAlpha);
		}
		else {
			rnewLocation = FMath::Lerp(targetLocation, rStartLocation, rAlpha);
		}

		rGlove->SetWorldLocation(rnewLocation);


		if (rAlpha >= 1.0f) {

			if (!rIsReturning) {
				rIsReturning = true;
				rAlpha = 0.0f;
			}
			else
			{
				rIsLerping = false;
				rIsReturning = false;
				rAlpha = 0.0f;
			}
		}
	}

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

}

// Called to bind functionality to input
void ATyson_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATyson_Character::leftHook()
{

	if (lIsLerping) return;
	lGlove->GetForwardVector() = Lorigin->GetForwardVector();

	lAlpha = 0.0f;
	lIsLerping = true;
	lIsReturning = false;
}

void ATyson_Character::rightHook()
{
	if (rIsLerping) return;
	rGlove->GetForwardVector() = Rorigin->GetForwardVector();


	rAlpha = 0.0f;
	rIsLerping = true;
	rIsReturning = false;
}


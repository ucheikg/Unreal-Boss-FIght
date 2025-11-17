// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBTTask_BlackboardBase.h"
#include "AiController.h"
#include "Tyson_Character.h"

UMoveBTTask_BlackboardBase::UMoveBTTask_BlackboardBase() {
	
    NodeName = TEXT("Close in");
}



EBTNodeResult::Type UMoveBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) return EBTNodeResult::Failed;

    if (Boss->withinRange = false) {
        
    }
    else {
        return EBTNodeResult::Succeeded;
    }

}

void ATyson_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (withinRange = false) {
        timeOut += DeltaTime;
    }
    else {
        
    }

}


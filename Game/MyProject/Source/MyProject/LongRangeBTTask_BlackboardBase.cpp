// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeBTTask_BlackboardBase.h"
#include "Tyson_Character.h"
#include "AiController.h"

// //give node a name
ULongRangeBTTask_BlackboardBase::ULongRangeBTTask_BlackboardBase()
{
	NodeName = TEXT("Long Range Attack");

}


EBTNodeResult::Type ULongRangeBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
//checks if the boss is controlled by AI if not return failed if it is cast the boss
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) return EBTNodeResult::Failed;
// call the bosses left hook function and returns succeded
    Boss->leftHook();

    return EBTNodeResult::Succeeded;
}

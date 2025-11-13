// Fill out your copyright notice in the Description page of Project Settings.


#include "Tyson_BTTask_BlackboardBase.h"
#include "AiController.h"
#include "Tyson_Character.h"

UTyson_BTTask_BlackboardBase::UTyson_BTTask_BlackboardBase()
{
    NodeName = TEXT("Attack Player"); // This name shows up in the Behavior Tree editor
}



EBTNodeResult::Type UTyson_BTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) return EBTNodeResult::Failed;
    
    Boss->rightHook();

    return EBTNodeResult::Succeeded;
}


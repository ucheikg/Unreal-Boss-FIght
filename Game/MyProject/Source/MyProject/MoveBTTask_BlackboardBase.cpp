// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveBTTask_BlackboardBase.h"
#include "Tyson_Character.h"
#include "AiController.h"
#include "Kismet/GameplayStatics.h"
#include "Creed.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"


UMoveBTTask_BlackboardBase::UMoveBTTask_BlackboardBase() {
    NodeName = TEXT("Move Towards");
    bNotifyTick = true;
}

EBTNodeResult::Type UMoveBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) return EBTNodeResult::Failed;

    ACreed* creedCharacter = Cast<ACreed>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!creedCharacter) return EBTNodeResult::Failed;


    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(creedCharacter);
    MoveRequest.SetAcceptanceRadius(1000.0f);

    FNavPathSharedPtr NavPath;
    AIController->MoveTo(MoveRequest, &NavPath);

    return EBTNodeResult::InProgress;
}

void UMoveBTTask_BlackboardBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACreed* creedCharacter = Cast<ACreed>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!creedCharacter) {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    Boss->inRange;

    if (Boss->inRange) {
        Boss->timeOut = 0.0f;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }
    else {
        Boss->timeOut += DeltaSeconds;

        if (Boss->timeOut >= 5.0f) {
            Boss->timeOut = 0.0f;
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }
    }
}
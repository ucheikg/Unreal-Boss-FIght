// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveBTTask_BlackboardBase.h"
#include "Tyson_Character.h"
#include "AiController.h"
#include "Kismet/GameplayStatics.h"
#include "Creed.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

// gives node a name when looking in behaviour tree
UMoveBTTask_BlackboardBase::UMoveBTTask_BlackboardBase() {
    NodeName = TEXT("Move Towards");
    bNotifyTick = true;
}

EBTNodeResult::Type UMoveBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // checks if pawn is controlled by AI
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
    // cast boss character to a placeholder
    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) return EBTNodeResult::Failed;
// cast the player to a placeholder
    ACreed* creedCharacter = Cast<ACreed>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!creedCharacter) return EBTNodeResult::Failed;

// tells the AI to move toward the the player
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(creedCharacter);
    MoveRequest.SetAcceptanceRadius(80.0f);
// tells AI use nav mesh as a guide
    FNavPathSharedPtr NavPath;
    AIController->MoveTo(MoveRequest, &NavPath);
    
    return EBTNodeResult::InProgress;
}
// tick task which will be used every frame
void UMoveBTTask_BlackboardBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
   //checks for AI cotnroller on pawn
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }
// cast player to placeholder
    ACreed* creedCharacter = Cast<ACreed>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!creedCharacter) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

// tells pawn to move to player
    AIController->MoveToActor(creedCharacter, 50.0f, true, true, false, nullptr, true);
// cast the boss pawn
    ATyson_Character* Boss = Cast<ATyson_Character>(AIController->GetPawn());
    if (!Boss) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }
// check if the player is within range if so it stops, if not it continues moving
    if (Boss->inRange)
    {
        AIController->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else {
        // a timer is active in the background which activates when player is not in range
        // returns faild if not completed
        Boss->timeOut += DeltaSeconds;

        if (Boss->timeOut >= 5.0f) {
            Boss->timeOut = 0.0f;
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LongRangeBTTask_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ULongRangeBTTask_BlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
// defines a function which can be called outside the script
	ULongRangeBTTask_BlackboardBase();


protected:
// creates the function and sets parameters
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

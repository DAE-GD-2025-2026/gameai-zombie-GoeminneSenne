// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskGrabItemGoeminneSenne.generated.h"

/**
 * 
 */
UCLASS()
class GOEMINNESENNEZOMBIERUNTIME_API UBTTaskGrabItemGoeminneSenne : public UBTTaskNode
{
	GENERATED_BODY()
	

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

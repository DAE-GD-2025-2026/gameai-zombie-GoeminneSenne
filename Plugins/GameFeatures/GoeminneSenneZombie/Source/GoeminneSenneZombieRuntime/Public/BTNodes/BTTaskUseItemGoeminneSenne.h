// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Items/ItemType.h"
#include "BTTaskUseItemGoeminneSenne.generated.h"

/**
 * 
 */
UCLASS()
class GOEMINNESENNEZOMBIERUNTIME_API UBTTaskUseItemGoeminneSenne : public UBTTaskNode
{
	GENERATED_BODY()

	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Item Type")
	EItemType ItemType;

};

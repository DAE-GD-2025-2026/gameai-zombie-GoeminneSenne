// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Common/InventoryComponent.h"
#include "BTTaskShootZombieGoeminneSenne.generated.h"

/**
 * 
 */
UCLASS()
class GOEMINNESENNEZOMBIERUNTIME_API UBTTaskShootZombieGoeminneSenne : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* TaskMemory) override;
	
private:
	void RemoveItem(UInventoryComponent* Inventory, int SlotIndex, UBlackboardComponent* Blackboard) const;
	
};

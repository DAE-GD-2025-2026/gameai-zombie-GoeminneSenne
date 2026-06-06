// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskUseItemGoeminneSenne.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTTaskUseItemGoeminneSenne::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Current Inventory
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (not AIController) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (not Survivor) return EBTNodeResult::Failed;
	
	UInventoryComponent* Inventory = Survivor->GetComponentByClass<UInventoryComponent>();
	if (not Inventory) return EBTNodeResult::Failed;
	
	//Find the index of the requested item type
	const auto& InventorySlots = Inventory->GetInventory();
	for (int idx{0}; idx < InventorySlots.Num(); ++idx)
	{
		ABaseItem* Item = InventorySlots[idx];
		if (Item && Item->GetItemType() == ItemType)
		{
			bool bUseResult = Inventory->UseItem(idx);
			if (not bUseResult) return EBTNodeResult::Failed;
			
			//Remove item from inventory if it has no value anymore
			if (Item->GetValue() <= 0)
			{
				Inventory->RemoveItem(idx);
			}
			
			return EBTNodeResult::Succeeded;
		}
	}
	
	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Red, TEXT("Could not find component of specific type") );
	return EBTNodeResult::Failed;
	
	
	return EBTNodeResult::Failed;
}

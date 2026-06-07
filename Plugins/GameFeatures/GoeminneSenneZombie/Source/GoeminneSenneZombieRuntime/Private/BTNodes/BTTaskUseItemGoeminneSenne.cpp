// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskUseItemGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
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
				
				//Check if another object of the same type is still in inventory
				bool bIsTypeInInventory = false;
				for (int ContinueIndex{idx}; ContinueIndex < Inventory->GetInventory().Num(); ++ContinueIndex)
				{
					if (InventorySlots[ContinueIndex] && InventorySlots[ContinueIndex]->GetItemType() == ItemType)
					{
						bIsTypeInInventory = true;
						break;
					}
				}
				
				//Update the blackboard fields for HasMedkit/HasFood
				UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
				if (not Blackboard) break;
				
				switch (ItemType)
				{
				case EItemType::Medkit:
					Blackboard->SetValueAsBool("HasMedkit", bIsTypeInInventory);
					break;
				case EItemType::Food:
					Blackboard->SetValueAsBool("HasFood", bIsTypeInInventory);
					break;
				default:
					break;
				}
				
			}
			
			return EBTNodeResult::Succeeded;
		}
	}
	
	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Red, TEXT("Could not find component of specific type") );
	return EBTNodeResult::Failed;
}

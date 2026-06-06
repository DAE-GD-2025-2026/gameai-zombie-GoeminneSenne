// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskGrabItemGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"

EBTNodeResult::Type UBTTaskGrabItemGoeminneSenne::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AI Controller & Blackboard
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (not AIController or not Blackboard) return EBTNodeResult::Failed;
		
	//Item
	ABaseItem* SpottedItem = Cast<ABaseItem>(Blackboard->GetValueAsObject("SpottedItem"));
	if (not IsValid(SpottedItem))
	{
		Blackboard->ClearValue("SpottedItem");
		return EBTNodeResult::Failed;
	}
	
	//If the spotted item is garbage, destroy it
	if (SpottedItem->GetItemType() == EItemType::Garbage)
	{
		SpottedItem->Destroy();
		Blackboard->ClearValue("SpottedItem");
		return EBTNodeResult::Succeeded;
	}
	
	//Survivor & Inventory
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (not IsValid(Survivor)) return EBTNodeResult::Failed;
	
	UInventoryComponent* Inventory = Survivor->GetComponentByClass<UInventoryComponent>();
	if (not Inventory) return EBTNodeResult::Failed;
	
	//Find empty inventory slot
	const auto& InventorySlots = Inventory->GetInventory();
	int SlotIndex = -1;
	
	for (int idx{0}; idx < InventorySlots.Num(); ++idx)
	{
		if (InventorySlots[idx] == nullptr)
		{
			SlotIndex = idx;
			break;
		}
	}
	
	if (SlotIndex == -1)
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red, TEXT("Full Inventory, can't grab item"));
		return EBTNodeResult::Failed;
	}
	
	//Add Item to inventory
	bool bGrabResult = Inventory->GrabItem(SlotIndex, SpottedItem);
	if (not bGrabResult) return EBTNodeResult::Failed;
	
	//Reset Blackboard fields
	Blackboard->ClearValue("SpottedItem");
	Blackboard->ClearValue("ItemLocation");
	return EBTNodeResult::Succeeded;
}

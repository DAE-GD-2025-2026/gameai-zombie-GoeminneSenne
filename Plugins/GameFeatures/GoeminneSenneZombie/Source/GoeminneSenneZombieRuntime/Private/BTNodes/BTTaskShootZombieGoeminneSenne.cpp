// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskShootZombieGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombies/BaseZombie.h"

EBTNodeResult::Type UBTTaskShootZombieGoeminneSenne::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* TaskMemory)
{
	//Get Current Inventory
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (not AIController) return EBTNodeResult::Failed;
	
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (not Survivor) return EBTNodeResult::Failed;
	
	UInventoryComponent* Inventory = Survivor->GetComponentByClass<UInventoryComponent>();
	if (not Inventory) return EBTNodeResult::Failed;
	
	//Get Spotted Zombie from Blackboard
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (not Blackboard) return EBTNodeResult::Failed;
	ABaseZombie* ZombieTarget = Cast<ABaseZombie>(Blackboard->GetValueAsObject("SpottedZombie"));
	if (not ZombieTarget) return EBTNodeResult::Failed;
	
	//If Zombie is close enough, prefer Shotgun, otherwise prefer Pistol
	EItemType PreferredWeapon = EItemType::Pistol;
	float DistSqr = FVector::DistSquared(ZombieTarget->GetActorLocation(), Survivor->GetActorLocation());
	if (DistSqr <= 300 * 300)
	{
		PreferredWeapon = EItemType::Shotgun;
	}
	
	//Check if the preferred Weapon Type is in Inventory
	const auto& InventorySlots = Inventory->GetInventory();
	int SlotIndex = -1;
	
	for (int idx{0}; idx < InventorySlots.Num(); ++idx)
	{
		ABaseItem* Item = InventorySlots[idx];
		if (Item && Item->GetItemType() == PreferredWeapon)
		{
			SlotIndex = idx;
			break;
		}
	}
	
	//If preferred weapon is not found, search for any weapon
	if (SlotIndex == -1)
	{
		for (int idx{0}; idx < InventorySlots.Num(); ++idx)
		{
			ABaseItem* Item = InventorySlots[idx];
			if (Item && (Item->GetItemType() == EItemType::Pistol || Item->GetItemType() == EItemType::Shotgun))
			{
				SlotIndex = idx;
				break;
			}
		}
	}
	
	//Still didn't find a weapon, fail
	if (SlotIndex == -1) return EBTNodeResult::Failed;
	
	ABaseItem* Item = InventorySlots[SlotIndex];
	bool bUseResult = Inventory->UseItem(SlotIndex);
	
	if (not bUseResult) return EBTNodeResult::Failed;
	
	//Remove Item from inventory if it has no value anymore
	if (Item->GetValue() <= 0)
	{
		RemoveItem(Inventory, SlotIndex, Blackboard);
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTaskShootZombieGoeminneSenne::RemoveItem(UInventoryComponent* Inventory, int SlotIndex,
	UBlackboardComponent* Blackboard) const
{
	Inventory->RemoveItem(SlotIndex);
	
	//Check if another weapon is still in inventory
	bool bIsWeaponInventory = false;
	for (int idx{0}; idx < Inventory->GetInventory().Num(); idx++)
	{
		ABaseItem* Item = Inventory->GetInventory()[idx];
		if (Item && (Item->GetItemType() == EItemType::Pistol || Item->GetItemType() == EItemType::Shotgun))
		{
			bIsWeaponInventory = true;
			break;
		}
	}
	
	//Update blackboard
	Blackboard->SetValueAsBool("HasWeapon", bIsWeaponInventory);
}

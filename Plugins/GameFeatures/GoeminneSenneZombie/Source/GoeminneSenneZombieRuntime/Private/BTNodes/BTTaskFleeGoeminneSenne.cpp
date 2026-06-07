// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskFleeGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskFleeGoeminneSenne::UBTTaskFleeGoeminneSenne()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskFleeGoeminneSenne::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (not Blackboard) return EBTNodeResult::Failed;
	
	//Retrieve Zombie from blackboard and save it as a data member
	ABaseZombie* Zombie = Cast<ABaseZombie>(Blackboard->GetValueAsObject("SpottedZombie"));
	if (not Zombie) return EBTNodeResult::Failed;
	SpottedZombie = Zombie;
	
	return EBTNodeResult::InProgress;
}

void UBTTaskFleeGoeminneSenne::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (not AIController) return;
	
	APawn* Pawn = AIController->GetPawn();
	if (not Pawn) return;
	
	//Check if Zombie still exists
	if (not SpottedZombie) return;
	
	//Move away from Zombie
	FVector FleeMovement = Pawn->GetActorLocation() - SpottedZombie->GetActorLocation();
	FleeMovement.Z = 0;
	
	GEngine->AddOnScreenDebugMessage(7, 1.f, FColor::Magenta, FString::Printf(TEXT("%f %f"), FleeMovement.X, FleeMovement.Y));

	Pawn->AddMovementInput(FleeMovement);	
}

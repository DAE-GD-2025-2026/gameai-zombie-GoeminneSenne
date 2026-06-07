// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNodes/BTTaskFleeGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survivor/SurvivorPawn.h"

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
	
	//Start Sprinting
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (not AIController) return EBTNodeResult::Failed;
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (not Survivor) return EBTNodeResult::Failed;
	
	//Only Sprint if we have enough stamina to keep moving
	if (Blackboard->GetValueAsFloat("Stamina") >= 3)
	Survivor->StartRunning();
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, FString::Printf(TEXT("Started Sprinting")));
	
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
	
	GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Magenta, FString::Printf(TEXT("Flee Movement: %f %f"), FleeMovement.X, FleeMovement.Y));

	Pawn->SetActorRotation(FleeMovement.Rotation());
	Pawn->AddMovementInput(FleeMovement);	
	
	//Check if we are far enough away from Chasing Zombie
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (not Blackboard) return;
	if (CheckSafeDistance(Pawn, Blackboard))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTaskFleeGoeminneSenne::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
	if (not Survivor) return EBTNodeResult::Failed;
	Survivor->StopRunning();
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Orange, FString::Printf(TEXT("Stopped Sprinting")));
	return EBTNodeResult::Aborted;
}

bool UBTTaskFleeGoeminneSenne::CheckSafeDistance(APawn* Pawn, UBlackboardComponent* Blackboard)
{
	if (FVector::DistSquared(Pawn->GetActorLocation(), SpottedZombie->GetActorLocation()) >= SafeDistance * SafeDistance)
	{
		Blackboard->ClearValue("SpottedZombie");
		ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);
		if (not Survivor) return false;
		
		Survivor->StopRunning();
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Orange, FString::Printf(TEXT("Stopped Sprinting")));
		return true;
	}
	
	return false;
}

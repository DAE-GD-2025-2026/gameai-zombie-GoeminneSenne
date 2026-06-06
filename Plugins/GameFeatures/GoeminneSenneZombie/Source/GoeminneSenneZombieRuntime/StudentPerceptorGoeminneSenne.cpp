// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorGoeminneSenne.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"
#include "Common/InventoryComponent.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"


UStudentPerceptorGoeminneSenne::UStudentPerceptorGoeminneSenne()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptorGoeminneSenne::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorGoeminneSenne::OnPerceptionUpdated);
	}
	
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (AAIController* Controller = Cast<AAIController>(Pawn->GetController()))
		{
			m_pBlackboard = Controller->GetBlackboardComponent();
		}
	}
	
	m_pHealth = GetOwner()->GetComponentByClass<UHealthComponent>();
	m_pStamina = GetOwner()->GetComponentByClass<UStaminaComponent>();
	m_pInventory = GetOwner()->GetComponentByClass<UInventoryComponent>();
}

void UStudentPerceptorGoeminneSenne::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{	
	if (m_pBlackboard == nullptr) return;
	if (not Stimulus.WasSuccessfullySensed()) return;

	// --- House ---------
	if (AHouse* House = Cast<AHouse>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Green,
			FString::Printf(TEXT("Saw House!")));
		
		//Check if House was already visited
		if (m_VisitedHouses.Contains(House))
		{
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Yellow, FString::Printf(TEXT("Already visited spotted house")));
			return;
		}
		
		m_VisitedHouses.Add(House);
		
		m_pBlackboard->SetValueAsObject("SpottedHouse", House);
		m_pBlackboard->SetValueAsVector("HouseLocation", House->GetBounds().Origin);
		
		return;
	}
	
	// --- Items ---------
	if (ABaseItem* Item = Cast<ABaseItem>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Green,
	FString::Printf(TEXT("Saw %s"), *ItemTypeToString(Item->GetItemType())));
		
		m_pBlackboard->SetValueAsObject("SpottedItem", Item);
		m_pBlackboard->SetValueAsVector("ItemLocation", Item->GetActorLocation());
		
		return;
	}
	
	// --- Zombie ---------
	if (ABaseZombie* Zombie = Cast<ABaseZombie>(Actor))
	{
		GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Green,
	FString::Printf(TEXT("Saw Zombie!")));
		
		return;
	}
	
}

FString UStudentPerceptorGoeminneSenne::ItemTypeToString(EItemType ItemType) const
{
	switch (ItemType)
	{
	case EItemType::Garbage:
		return TEXT("Garbage");
	case EItemType::Food:
		return TEXT("Food");
	case EItemType::Medkit:
		return TEXT("Medkit");
	case EItemType::Pistol:
		return TEXT("Pistol");
	case EItemType::Shotgun:
		return TEXT("Shotgun");
	default:
		break;
	}
	
	return TEXT("Unknown Item Type");
}

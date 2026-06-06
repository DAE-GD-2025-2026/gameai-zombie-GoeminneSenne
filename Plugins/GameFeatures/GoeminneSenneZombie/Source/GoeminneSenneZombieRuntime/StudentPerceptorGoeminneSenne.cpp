// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorGoeminneSenne.h"

#include "AIController.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"
#include "Common/InventoryComponent.h"


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
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
}

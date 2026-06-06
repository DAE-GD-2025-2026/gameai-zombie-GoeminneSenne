// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptorGoeminneSenne.generated.h"

class AHouse;
enum class EItemType : uint8;
class UHealthComponent;
class UStaminaComponent;
class UInventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GOEMINNESENNEZOMBIERUNTIME_API UStudentPerceptorGoeminneSenne : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptorGoeminneSenne();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
private:
	UBlackboardComponent* m_pBlackboard{nullptr};
	UHealthComponent* m_pHealth{nullptr};
	UStaminaComponent* m_pStamina{nullptr};
	UInventoryComponent* m_pInventory{nullptr};
	
	TArray<AHouse*> m_VisitedHouses{};
	
	
	FString ItemTypeToString(EItemType ItemType) const;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorGoeminneSenne.h"


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
}

void UStudentPerceptorGoeminneSenne::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
}

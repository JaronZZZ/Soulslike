// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Strafe.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UBT_Strafe : public UBTTaskNode
{
	GENERATED_BODY()
	

	class AEnemyAIController* AIController;

	class ABaseEnemy* AICharacter;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void EndStrafe();

	FTimerHandle DelayEndStrafe;

};

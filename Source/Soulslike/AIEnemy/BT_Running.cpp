// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Running.h"

#include "BaseEnemy.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBT_Running::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AEnemyAIController* AIController=Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if(ABaseEnemy* AICharacter=Cast<ABaseEnemy>(AIController->GetPawn()))
		{
			AICharacter->RunningMovement(true);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

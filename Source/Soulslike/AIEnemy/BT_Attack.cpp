// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Attack.h"

#include "BaseEnemy.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("YesAttack"));

	if(AEnemyAIController* AIController=Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if(ABaseEnemy* AICharacter=Cast<ABaseEnemy>(AIController->GetPawn()))
		{
			// UE_LOG(LogTemp,Warning,TEXT("Attack"));
			AICharacter->Attack();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_UnEquip.h"

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Soulslike/Character/SoulBaseCharacter.h"

EBTNodeResult::Type UBT_UnEquip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (ASoulBaseCharacter* Player = Cast<ASoulBaseCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetPlayer")))
		{
			AIController->SetFocus(Player);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Succeeded;
}

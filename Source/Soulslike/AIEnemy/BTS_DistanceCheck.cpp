// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_DistanceCheck.h"

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Soulslike/Character/SoulBaseCharacter.h"

void UBTS_DistanceCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if(AEnemyAIController* AIController=Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if(ABaseEnemy* AICharacter=Cast<ABaseEnemy>(AIController->GetPawn()))
		{
			if(ASoulBaseCharacter* Player=Cast<ASoulBaseCharacter>(AICharacter->TargetPlayer))
			{
				float Distance =AICharacter->GetDistanceTo(Player);
				if(Distance<=250.f)
				{
					AIController->GetBlackboardComponent()->SetValueAsBool("IsAttack",true);
				}
				else
				{
					if(Distance>1000.f)
					{
						AIController->GetBlackboardComponent()->SetValueAsBool("IsRunning",true);
					}	
					AIController->GetBlackboardComponent()->SetValueAsBool("IsAttack",false);
				}
			}
		}
	}
}

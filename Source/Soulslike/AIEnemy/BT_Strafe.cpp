// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Strafe.h"

#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


EBTNodeResult::Type UBT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController=Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if(AIController)
	{
		AICharacter=Cast<ABaseEnemy>(AIController->GetPawn());
		if(AICharacter)
		{
			if(!AIController->GetBlackboardComponent()->GetValueAsBool("StrafeDoOnce"))
			{
				// UE_LOG(LogTemp,Error,TEXT("StrafeDoOnce"));
				AIController->GetBlackboardComponent()->SetValueAsBool("StrafeDoOnce",true);
				AICharacter->SetActorTickEnabled(true);
				GetWorld()->GetTimerManager().SetTimer(DelayEndStrafe,this,&UBT_Strafe::EndStrafe,
					UKismetMathLibrary::RandomFloatInRange(5.f,10.f),
					false,UKismetMathLibrary::RandomFloatInRange(5.f,10.f));
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

void UBT_Strafe::EndStrafe()
{
	AIController->GetBlackboardComponent()->SetValueAsBool("IsStrafe",false);
	AIController->GetBlackboardComponent()->SetValueAsBool("StrafeDoOnce",false);
	AICharacter->SetActorTickEnabled(false);

	AICharacter->RushAttack();
	
}

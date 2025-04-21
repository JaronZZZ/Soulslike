// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Soulslike/Character/SoulBaseCharacter.h"
#include "Soulslike/Logic/SoulEventManager.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), "INSWORD");

	HitColl = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitColl"));
	HitColl->SetupAttachment(Sword);
	HitColl->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitColl->SetCollisionResponseToChannels(ECR_Ignore);
	HitColl->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bEquip = false;
	bInjury = false;
	bAttack = false;
	bRunning = false;
	bIgnoreHit = false;
	bDie = false;
	bRolling = false;


	MaxHP = 100.0f;
	HP = MaxHP;
	TargetPlayer = nullptr;
	LastEquipIndex = 0;
	StrafeMoveValue = 0;
	RushAttackPos = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	GetWorld()->GetTimerManager().SetTimer(DelayRandomStrafe, this, &ABaseEnemy::EnableStrafe,
	                                       UKismetMathLibrary::RandomFloatInRange(15.f, 20.f), true, UKismetMathLibrary::RandomFloatInRange(15.f, 20.f));

	GetWorld()->GetTimerManager().SetTimer(DelayFindPlayerPtr, this, &ABaseEnemy::FindTargetPlayer,
	                                       1.f, false, 1.f);
	bIgnoreHit = true;
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//解绑
	CanGetEnemyTarget.Clear();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StrafeMovement(StrafeMoveValue);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::FindTargetPlayer()
{
	while (TargetPlayer == nullptr)
	{
		if(AActor* FindActor = UGameplayStatics::GetActorOfClass(GetWorld(), InsPlayer))
		{
			TargetPlayer = Cast<ASoulBaseCharacter>(FindActor);
		}
	}
	if (TargetPlayer)
	{
		EnemyAIController = Cast<AEnemyAIController>(GetController());
		EnemyAIController->GetBlackboardComponent()->SetValueAsObject("TargetPlayer", TargetPlayer);
	}
}

void ABaseEnemy::RunningMovement(bool IsRun)
{
	if (IsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

float ABaseEnemy::GetEnemyCurHP()
{
	return HP;
}

float ABaseEnemy::GetEnemyMaxHP()
{
	return MaxHP;
}

void ABaseEnemy::MontageRandomPlay(TArray<UAnimMontage*>& AnimMontageArray, int32& LastIndex)
{
	int32 PlayIndex = UKismetMathLibrary::RandomIntegerInRange(0, AnimMontageArray.Num() - 1);
	if (UAnimInstance* CurAnimInstance = GetMesh()->GetAnimInstance())
	{
		if (PlayIndex != LastIndex)
		{
			CurAnimInstance->Montage_Play(AnimMontageArray[PlayIndex]);
		}
		else
		{
			if (PlayIndex == 0)
			{
				int32 AddIndex = UKismetMathLibrary::RandomIntegerInRange(0, AnimMontageArray.Num() - 1);
				PlayIndex = AddIndex + LastIndex;
				LastIndex = PlayIndex;
				CurAnimInstance->Montage_Play(AnimMontageArray[PlayIndex]);
			}
			else
			{
				LastIndex = --PlayIndex;
				CurAnimInstance->Montage_Play(AnimMontageArray[PlayIndex]);
			}
		}
	}
}

void ABaseEnemy::DrawTheSword()
{
	CanGetEnemyTarget.Broadcast();

	if (bEquip == false)
	{
		MontageRandomPlay(EquipSwordAnim, LastEquipIndex);
		bIgnoreHit = false;

		if (GetMesh()->GetAnimInstance())
		{
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WEAPON_R");
		}
	}
}

void ABaseEnemy::Attack()
{
	// GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Green,TEXT("YesAttack"));


	if (CanAttack())
	{
		EnableSwordCollision(false);
		bool RandomAttackBool = UKismetMathLibrary::RandomBoolWithWeight(0.7);
		if (RandomAttackBool)
		{
			// UE_LOG(LogTemp, Warning, TEXT("true"));
			bAttack = true;
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRunning", false);
			RunningMovement(false);
			MontageRandomPlay(AttackAnimArray, LastAttackIndex);
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("false"));
			Rolling();
		}
	}
}

void ABaseEnemy::Rolling()
{
	if (UAnimInstance* CurAnimInstance = GetMesh()->GetAnimInstance())
	{
		bAttack = false;
		bInjury = false;
		bIgnoreHit = true;
		bRolling = true;
		int32 RollingIndex = UKismetMathLibrary::RandomIntegerInRange(0, 5);
		CurAnimInstance->Montage_Play(RollingAnimArray[RollingIndex]);
	}
}

void ABaseEnemy::RushAttack()
{
	if (CanRushAttack())
	{
		if (UAnimInstance* CurAnimInstance = GetMesh()->GetAnimInstance())
		{
			bAttack = true;
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRunning", false);
			RunningMovement(false);
			EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRushAttack", true);
			RushAttackPos = GetShiftingTargetPos();
			//这里RushAttack的位置确认偷懒了，想要做好的话可以给一个状态通知，在这个时间段内不断更新位置
			//然后超出时间以后就以最后的位置为位移地点，这样效果会好一些

			CurAnimInstance->Montage_Play(RushAttackAnim[0]);
		}
	}
}

void ABaseEnemy::RushAttackShifting(float Lerp)
{
	FVector LerpLoc = UKismetMathLibrary::VLerp(GetActorLocation(), RushAttackPos, Lerp);
	SetActorLocation(FVector(LerpLoc.X, LerpLoc.Y, GetActorLocation().Z));
}

void ABaseEnemy::EnableSwordCollision(bool IsOpen)
{
	if (IsOpen)
	{
		if (!HitColl->OnComponentBeginOverlap.IsBound())
		{
			HitColl->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::SwordOverlap);
		}
	}
	else
	{
		if (HitColl->OnComponentBeginOverlap.IsBound())
		{
			HitColl->OnComponentBeginOverlap.RemoveDynamic(this, &ABaseEnemy::SwordOverlap);
		}
	}
}

void ABaseEnemy::SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                              bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASoulBaseCharacter* PlayerCharacter = Cast<ASoulBaseCharacter>(OtherActor))
	{
		PlayerCharacter->Injury();
	}
}


bool ABaseEnemy::CanAttack()
{
	return bEquip && !bAttack && !bInjury && !bRolling;
}

bool ABaseEnemy::CanInjury()
{
	if (!bIgnoreHit)
	{
		return true;
	}
	return false;
}

bool ABaseEnemy::CanRushAttack()
{
	return bEquip && !bAttack && !bInjury && !bRolling;
}

void ABaseEnemy::ChangeBlackboardValue()
{
	bEquip = true;

	if (EnemyAIController)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsEquip", bEquip);
	}
}

void ABaseEnemy::Injury()
{
	if (CanInjury())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Injury");
		EnableSwordCollision(false);
		if (!bEquip)
		{
			ChangeBlackboardValue();
		}
		bInjury = true;
		MontageRandomPlay(InjuryAnimArray, LastInjuryIndex);
		if (HP <= 10)
		{
			HP -= 10;
			HP = 0;
			Die();
		}
		else
		{
			HP -= 10;
		}
	}
}

void ABaseEnemy::Die()
{
	EnableSwordCollision(false);
	bDie = true;
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsAnyoneDie", true);
	//关闭胶囊体碰撞
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	}
	DeathUI();
}

void ABaseEnemy::DeathUI()
{
	USoulEventManager::GetInstancePtr()->OpenFightResultUI.ExecuteIfBound(true);
}

void ABaseEnemy::EnableStrafe()
{
	if (bEquip)
	{
		EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsStrafe", true);
		RunningMovement(false);
		if (UKismetMathLibrary::RandomBool())
		{
			StrafeMoveValue = 1;
		}
		else
		{
			StrafeMoveValue = -1;
		}
		// UE_LOG(LogTemp, Warning, TEXT("%d"), StrafeMoveValue);
	}
}

void ABaseEnemy::StrafeMovement(int32 StrafeMoveDir)
{
	const FRotator LocalRotation = GetActorRotation();
	const FVector LocalVector = FRotationMatrix(FRotator(0, LocalRotation.Yaw, 0)).GetUnitAxis(EAxis::Y);
	AddMovementInput(LocalVector, StrafeMoveDir);
	// UE_LOG(LogTemp,Warning,TEXT("%d"),StrafeMoveDir);
}

FVector ABaseEnemy::GetShiftingTargetPos()
{
	return TargetPlayer->PlayerPosArrow->GetComponentLocation();
}

void ABaseEnemy::StopRushAttack()
{
	EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsRushAttack", false);
}

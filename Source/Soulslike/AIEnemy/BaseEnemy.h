// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Soulslike/SoulEnumType.h"
#include "BaseEnemy.generated.h"

class AEnemyAIController;
class ASoulBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyTargetGetFunction);

UCLASS()
class SOULSLIKE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Item)
	USkeletalMeshComponent* Sword;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Item)
	UBoxComponent* HitColl;

	//拔剑动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=AnimMontage)
	TArray<UAnimMontage*> EquipSwordAnim;

	//攻击动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=AnimMontage)
	TArray<UAnimMontage*> AttackAnimArray;

	//冲刺攻击动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=AnimMontage)
	TArray<UAnimMontage*> RushAttackAnim;

	//受击动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=AnimMontage)
	TArray<UAnimMontage*> InjuryAnimArray;

	//翻滚动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=AnimMontage)
	TArray<UAnimMontage*> RollingAnimArray;

	//是否已经装备
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bEquip;

	//是否跑步
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	bool bRunning;

	//是否攻击
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bAttack;

	//是否受击
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bInjury;

	//是否受击免疫
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bIgnoreHit;

	//是否死亡
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bDie;

	//是否翻滚
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Attribute)
	bool bRolling;
	
	//血量
	UPROPERTY(BlueprintReadOnly,Category=Attribute)
	float HP;

	//最大血量
	UPROPERTY(BlueprintReadOnly,Category=Attribute)
	float MaxHP;

	UFUNCTION(BlueprintPure,BlueprintCallable)
	float GetEnemyCurHP();

	UFUNCTION(BlueprintPure,BlueprintCallable)
	float GetEnemyMaxHP();

	//AIController
	AEnemyAIController* EnemyAIController;
	
	//玩家目标
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	ASoulBaseCharacter* TargetPlayer;

	//玩家参照
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Attribute)
	TSubclassOf<ASoulBaseCharacter> InsPlayer;

	//随机播放蒙太奇
	void MontageRandomPlay(TArray<UAnimMontage*>& AnimMontageArray,int32& LastIndex);

	//上次播放的拔剑蒙太奇序号
	int32 LastEquipIndex;

	//上次播放的受击蒙太奇序号
	int32 LastInjuryIndex;

	//上次播放的攻击蒙太奇序号
	int32 LastAttackIndex;

	//获取玩家目标
	void FindTargetPlayer();

	void RunningMovement(bool IsRun);

	//拔刀
	UFUNCTION(BlueprintCallable)
	void DrawTheSword();

	//攻击
	void Attack();

	//翻滚
	void Rolling();

	//冲刺攻击（用于观察者模式结束后）
	void RushAttack();

	UFUNCTION(BlueprintCallable)
	void RushAttackShifting(float Lerp);
	
	//开启/关闭刀剑碰撞检测
	UFUNCTION(BlueprintCallable)
	void EnableSwordCollision(bool IsOpen);
	
	UFUNCTION()
	void SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
					  const FHitResult& SweepResult);

	//是否可以攻击
	bool CanAttack();

	//是否可以受伤
	bool CanInjury();

	//是否可以冲刺攻击
	bool CanRushAttack();

	//修改黑板bEquip值
	UFUNCTION(BlueprintCallable)
	void ChangeBlackboardValue();

	//受击
	void Injury();

	//死亡
	void Die();

	//死亡显示UI
	void DeathUI();

	//蓝图委托
	UPROPERTY(BlueprintAssignable,Category=Event)
	FEnemyTargetGetFunction CanGetEnemyTarget;

	//启动观察者模式（左右走）
	void EnableStrafe();

	//观察者模式的移动
	void StrafeMovement(int32 StrafeMoveDir);

	//移动方向
	int32 StrafeMoveValue;

	FTimerHandle DelayRandomStrafe;
	FTimerHandle DelayFindPlayerPtr;
	
	//冲刺的目标位置
	FVector	RushAttackPos;
	
	//获取冲刺目标位置
	FVector GetShiftingTargetPos();

	//结束冲刺攻击状态（可以取消并且移动了，建立该状态主要是为了应对蓄力阶段会Moveto的情况）
	//并且该状态用在Moveto的时候反转设置，false可以移动，true不行
	UFUNCTION(BlueprintCallable)
	void StopRushAttack();


	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "Soulslike/SoulEnumType.h"
#include "SoulBaseCharacter.generated.h"

class UInputAction;
class ABaseEnemy;


UCLASS()
class SOULSLIKE_API ASoulBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoulBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	//摄像机
	UPROPERTY(EditDefaultsOnly,blueprintreadwrite,Category=Camera,meta=(AllowPrivateAccess="true"))
	class UCameraComponent* FollowCamera; 

	//弹簧臂
	UPROPERTY(EditDefaultsOnly,blueprintreadwrite,Category=CameraBoom,meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* CameraBoom;

public:
	//按键绑定
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* RollingAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* SwitchWeaponAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* SwordDefenseAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	UInputAction* LockViewAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UArrowComponent* PlayerPosArrow;
	
	//这里获取了敌人指针，方便进行交互
	UPROPERTY(BlueprintReadOnly,Category=PlayerInput)
	ABaseEnemy* EnemyTarget;

	//为敌人参照
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=PlayerInput)
	TSubclassOf<ABaseEnemy> InsEnemy;

	void FindEnemyPtr();
	

	//移动
	void Move(const FInputActionValue& Value);

	//视角
	virtual void Look(const FInputActionValue& Value);

	//攻击
	virtual void Attack(){};

	//空手攻击检测
	UFUNCTION(BlueprintCallable)
	void EnableMeleeCollision();
	
	//随机攻击蒙太奇播放
	void RandomAttack(TArray<UAnimMontage*> NeedRandomAnims, int32&	 LastIndex);

	//随机播放动画
	void RandomAnimMontage(TArray<UAnimMontage*> NeedRandomAnims);

	//播放翻滚/滑行动画
	void PlayRollingOrSlideMontage(TArray<UAnimMontage*> RollingOrSlideAnimArray);

	//翻滚
	virtual void Rolling(){};

	//滑步（翻跟头）
	virtual void Slide(){};

	//受击
	virtual void Injury(){};

	//持剑防御
	virtual void SwordDefense(){};

	//结束持剑防御
	virtual void StopSwordDefense(){};

	//切换武器
	virtual void SwitchWeapon(){};

	//奔跑
	void Run();
	
	//是否可以奔跑
	bool CanRun();

	//停止奔跑
	void StopRun();

	//死亡
	void Die();

	//死亡UI
	void DeathUI();

	//移除玩家输入
	void RemovePlayerInput();

	//获取当前生命值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetCurHp();

	//获取最大生命值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetMaxHP();

	//是否处于奔跑状态
	UPROPERTY(BlueprintReadOnly,Category=Attribute)
	bool bRunning;

	//是否装备有武器
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	bool bEquip;

	//是否死亡
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	bool bDie;

	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	bool bIgnoreHit;

	//武器类型
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	EWeaponType	WeaponType;

	//空手状态
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	EMeleeType MeleeType;

	//角色行为
	UPROPERTY(BlueprintReadWrite,Category=Attribute)
	EPlayerBehavior PlayerBehavior;
	
	//生命值
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float PlayerHP;
	
	//最大生命值
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float MaxPlayerHP;

	//体力值
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float Stamina;

	//最大体力值
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float MaxStamina;

	//空手攻击消耗体力
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float MeleeAttackSubStamina;

	//武器攻击消耗体力
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float SwordAttackSubStamina;

	//空手翻滚消耗体力
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float MeleeRollingSubStamina;

	//持剑翻滚消耗体力
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float SwordRollingSubStamina;

	//体力恢复系数（以帧为单位时间）
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float IncreaseStamina;

	//空手攻击框半径
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=Attribute)
	float MeleeSphereCollisionRadius;

	//获取当前人物的体力值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetCurStamina();

	//获取当前人物最大的体力值
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetMaxStamina();

	//摄像机震动
	UFUNCTION(BlueprintImplementableEvent)
	void CameraShakeFeedBack(bool IsOpen);
	
	//攻击时转向的目标值
	FRotator DesiredRotation;
	
	//计算按键方向
	FRotator CalculateRotation();

	//给插值旋转（或者说是过程旋转），为了使旋转效果看起来平滑一点
	UFUNCTION(BlueprintCallable)			
	void RInterpRotation();

	//翻滚朝向，也就是往哪个方向翻滚（前后）
	int32 RollingForwardValue;

	//左右翻滚朝向
	int32 RollingRightValue;

	//翻滚动画播放速率
	float RollingAnimPlayRate;

	//处理随机播放攻击蒙太奇
	int32 LastMeleeIndex;

	int32 LastSwordIndex;
	
};

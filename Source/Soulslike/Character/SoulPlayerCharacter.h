// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soulslike/Character/SoulBaseCharacter.h"
#include "SoulPlayerCharacter.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class SOULSLIKE_API ASoulPlayerCharacter : public ASoulBaseCharacter
{
	GENERATED_BODY()

public:
	ASoulPlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//考虑到锁定视角的问题，需要对Look函数重载一下
	virtual void Look(const FInputActionValue& Value) override;
	
	//攻击行为
	virtual void Attack() override;

	//空手攻击
	void MeleeAttack();

	//是否可以进行空手攻击
	bool CanMeleeAttack();

	//武器攻击（刀）
	void SwordAttack();

	//是否可以进行武器攻击
	bool CanSwordAttack();

	//开启/关闭刀剑碰撞检测
	UFUNCTION(BlueprintCallable)
	void EnableSwordCollision(bool IsOpen);
	
	UFUNCTION()
	void SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                  const FHitResult& SweepResult);


	//翻滚行为
	virtual void Rolling() override;

	//空手翻滚
	void MeleeRolling();

	//是否可以进行空手翻滚
	bool CanMeleeRolling();

	//持剑翻滚
	void SwordRolling();

	//是否可以持剑翻滚
	bool CanSwordRolling();


	//滑步（翻跟头）
	virtual void Slide() override;

	//空手翻跟头
	void MeleeSlide();

	//是否可以空手翻跟头
	bool CanMeleeSlide();

	//持剑滑步
	void SwordSlide();

	//是否可以持剑滑步
	bool CanSwordSlide();


	//更换武器
	virtual void SwitchWeapon() override;

	//是否可以切换武器
	bool CanSwitchWeapon();


	//持剑防御
	virtual void SwordDefense() override;

	//结束持剑防御
	virtual void StopSwordDefense() override;

	//是否可以持剑防御
	bool CanSwordDefense();

	//受击
	virtual void Injury() override;

	//空手受击
	void MeleeInjury();

	//持剑受击
	void SwordInjury();

	//格挡受击
	void DefenseInjury();

	//当前是否可以受击
	bool CanInjury();

	//做一个锁定视角切换按键
	void SwitchLockView();

	//改变视角逻辑
	UFUNCTION(BlueprintCallable)
	void ViewChange();
	
	UPROPERTY(BlueprintReadWrite)
	bool isLock;
public:
	//剑鞘的插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* SwordCover_L;

	//剑的插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Sword;

	//剑风
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	UNiagaraComponent* SwordQi;

	//头
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Head;

	//头发
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Hair;

	//帽子
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Hat;

	//裤子
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Pants;

	//左手
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* LeftHand;

	//右手
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* RightHand;

	//鞋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* Shoes;

	//剑的碰撞检测组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Item, meta=(AllowPrivateAccess=true))
	UBoxComponent* SwordColl;

	//空手攻击蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> MeleeAttackAnimArray;

	//武器攻击蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> SwordAttackAnimArray;

	//空手翻滚蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> MeleeRollingAnimArray;

	//空手翻跟头蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> MeleeSlideAnimArray;

	//持剑翻滚蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> SwordRollingAnimArray;

	//持剑滑步蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> SwordSlideAnimArray;

	//空手受击蒙太奇动画列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> MeleeInjuryAnimArray;

	//持剑受击蒙太奇动作列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> SwordInjuryAnimArray;

	//格挡受击蒙太奇动作列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AnimMontage, meta=(AllowPrivateAccess=true))
	TArray<UAnimMontage*> DefenseInjuryAnimArray;
	
	//做一个倒计时，用作切换备战和常态的状态
	float MeleeTypeWarToCommon;
};

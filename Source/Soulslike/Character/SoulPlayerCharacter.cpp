// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPlayerCharacter.h"
#include "Soulslike/Character/SoulPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "SoulGameInstance.h"
#include "SoulPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Soulslike/AIEnemy/BaseEnemy.h"
#include "Soulslike/Logic/SoulEventManager.h"

ASoulPlayerCharacter::ASoulPlayerCharacter()
{
	SwordCover_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player_SwordCover"));
	SwordCover_L->SetupAttachment(GetMesh(), "SwordCover_L");

	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerSword"));
	Sword->SetupAttachment(GetMesh(), "Sword");

	SwordQi = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SwordQi"));
	SwordQi->SetupAttachment(Sword);

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHead"));
	Head->SetupAttachment(GetMesh());

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHair"));
	Hair->SetupAttachment(GetMesh(), "HAIR");

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerHat"));
	Hat->SetupAttachment(GetMesh(), "HAIR");

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerLeftHand"));
	LeftHand->SetupAttachment(GetMesh());

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerRightHand"));
	RightHand->SetupAttachment(GetMesh());

	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerPants"));
	Pants->SetupAttachment(GetMesh());

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerShoes"));
	Shoes->SetupAttachment(GetMesh());

	SwordColl = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordColl->SetupAttachment(Sword);

	MeleeTypeWarToCommon = 5.0f;

	isLock = false;
}

void ASoulPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	if (USoulGameInstance* GameInstance = Cast<USoulGameInstance>(GetGameInstance()))
	{
		if (GameInstance->TopCloth)
		{
			GetMesh()->SetSkeletalMeshAsset(GameInstance->TopCloth);
		}
		if (GameInstance->BotCloth)
		{
			Pants->SetSkeletalMeshAsset(GameInstance->BotCloth);
		}
		if (GameInstance->Hat)
		{
			Hat->SetSkeletalMeshAsset(GameInstance->Hat);
		}
		if (GameInstance->Shoes)
		{
			Shoes->SetSkeletalMeshAsset(GameInstance->Shoes);
		}
	}
}

void ASoulPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//体力恢复模块，但此处最好还是做一个独立函数
	if (Stamina < 100.0f && PlayerBehavior != EPlayerBehavior::ATTACK && PlayerBehavior != EPlayerBehavior::DEFENSE)
	{
		// GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Black,FString::Printf(TEXT("StaminaNow=%f"),Stamina));
		Stamina += DeltaTime * IncreaseStamina;
	}

	//空手备战——常态状态切换时间参数
	if (MeleeType == EMeleeType::PREPAREWAR && PlayerBehavior != EPlayerBehavior::ATTACK)
	{
		MeleeTypeWarToCommon -= DeltaTime;
		// GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Black,FString::Printf(TEXT("MeleeTypeWarToCommon=%f"),MeleeTypeWarToCommon));
		if (MeleeTypeWarToCommon <= 0)
		{
			MeleeType = EMeleeType::COMMON;
			MeleeTypeWarToCommon = 5.0f;
		}
	}

	//锁定视角
	if(isLock)
	{
		ViewChange();
	}
}

void ASoulPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::Look);
		}
		if (RunAction)
		{
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::Run);
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASoulPlayerCharacter::StopRun);
		}
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Attack);
		}
		if (RollingAction)
		{
			EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Rolling);
		}
		if (SlideAction)
		{
			EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::Slide);
		}
		if (SwitchWeaponAction)
		{
			EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::SwitchWeapon);
		}
		if (SwordDefenseAction)
		{
			EnhancedInputComponent->BindAction(SwordDefenseAction, ETriggerEvent::Triggered, this, &ASoulPlayerCharacter::SwordDefense);
			EnhancedInputComponent->BindAction(SwordDefenseAction, ETriggerEvent::Completed, this, &ASoulPlayerCharacter::StopSwordDefense);
		}
		if (LockViewAction)
		{
			EnhancedInputComponent->BindAction(LockViewAction, ETriggerEvent::Started, this, &ASoulPlayerCharacter::SwitchLockView);
		}
	}
}

void ASoulPlayerCharacter::Look(const FInputActionValue& Value)
{
	if(!isLock)
	{
		Super::Look(Value);
	}
}

void ASoulPlayerCharacter::Attack()
{
	switch (WeaponType)
	{
	case EWeaponType::MELEE:
		MeleeAttack();
		break;
	case EWeaponType::SWORD:
		SwordAttack();
		break;
	}
}

void ASoulPlayerCharacter::MeleeAttack()
{
	if (CanMeleeAttack())
	{
		PlayerBehavior = EPlayerBehavior::ATTACK;
		MeleeType = EMeleeType::PREPAREWAR;
		MeleeTypeWarToCommon = 5.0f;
		Stamina -= MeleeAttackSubStamina;
		CameraShakeFeedBack(false);

		RandomAttack(MeleeAttackAnimArray, LastMeleeIndex);
	}
}


bool ASoulPlayerCharacter::CanMeleeAttack()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeAttackSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::SwordAttack()
{
	if (CanSwordAttack())
	{
		PlayerBehavior = EPlayerBehavior::ATTACK;
		Stamina -= SwordAttackSubStamina;
		CameraShakeFeedBack(false);

		RandomAttack(SwordAttackAnimArray, LastSwordIndex);
	}
}

bool ASoulPlayerCharacter::CanSwordAttack()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordAttackSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::EnableSwordCollision(bool IsOpen)
{
	if (IsOpen)
	{
		if (!SwordColl->OnComponentBeginOverlap.IsBound())
		{
			SwordColl->OnComponentBeginOverlap.AddDynamic(this, &ASoulPlayerCharacter::SwordOverlap);
		}
	}
	else
	{
		if (SwordColl->OnComponentBeginOverlap.IsBound())
		{
			SwordColl->OnComponentBeginOverlap.RemoveDynamic(this, &ASoulPlayerCharacter::SwordOverlap);
		}
	}
}

void ASoulPlayerCharacter::SwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                        bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		Enemy->Injury();
	}
}


void ASoulPlayerCharacter::Rolling()
{
	switch (WeaponType)
	{
	case EWeaponType::MELEE:
		MeleeRolling();
		break;
	case EWeaponType::SWORD:
		SwordRolling();
		break;
	}
}

void ASoulPlayerCharacter::MeleeRolling()
{
	if (CanMeleeRolling())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= MeleeRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;

		PlayRollingOrSlideMontage(MeleeRollingAnimArray);
	}
}

bool ASoulPlayerCharacter::CanMeleeRolling()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeRollingSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::SwordRolling()
{
	if (CanSwordRolling())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= SwordRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;

		PlayRollingOrSlideMontage(SwordRollingAnimArray);
	}
}

bool ASoulPlayerCharacter::CanSwordRolling()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordRollingSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ASoulPlayerCharacter::Slide()
{
	switch (WeaponType)
	{
	case EWeaponType::MELEE:
		MeleeSlide();
		break;
	case EWeaponType::SWORD:
		SwordSlide();
		break;
	}
}

void ASoulPlayerCharacter::MeleeSlide()
{
	if (CanMeleeSlide())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= MeleeRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;

		PlayRollingOrSlideMontage(MeleeSlideAnimArray);
	}
}

bool ASoulPlayerCharacter::CanMeleeSlide()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= MeleeRollingSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::SwordSlide()
{
	if (CanSwordSlide())
	{
		bIgnoreHit = true;
		CameraShakeFeedBack(false);
		Stamina -= SwordRollingSubStamina;
		PlayerBehavior = EPlayerBehavior::ROLLING;

		PlayRollingOrSlideMontage(SwordSlideAnimArray);
	}
}

bool ASoulPlayerCharacter::CanSwordSlide()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && Stamina >= SwordRollingSubStamina)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ASoulPlayerCharacter::SwitchWeapon()
{
	if (CanSwitchWeapon())
	{
		if (WeaponType == EWeaponType::MELEE)
		{
			WeaponType = EWeaponType::SWORD;
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WEAPON_R");
			USoulEventManager::GetInstancePtr()->UpdateWeaponText.ExecuteIfBound(WeaponType);
			bEquip = true;
		}
		else if (WeaponType == EWeaponType::SWORD)
		{
			WeaponType = EWeaponType::MELEE;
			Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Sword");
			USoulEventManager::GetInstancePtr()->UpdateWeaponText.ExecuteIfBound(WeaponType);
			bEquip = false;
		}
	}
}

bool ASoulPlayerCharacter::CanSwitchWeapon()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::SwordDefense()
{
	if (CanSwordDefense())
	{
		PlayerBehavior = EPlayerBehavior::DEFENSE;
		//重设速度，防止出现先按住跑再防御的速度错误
		StopRun();
	}
}

void ASoulPlayerCharacter::StopSwordDefense()
{
	if (PlayerBehavior == EPlayerBehavior::DEFENSE)
	{
		PlayerBehavior = EPlayerBehavior::IDLE;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
}

bool ASoulPlayerCharacter::CanSwordDefense()
{
	if (PlayerBehavior == EPlayerBehavior::IDLE && WeaponType == EWeaponType::SWORD)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASoulPlayerCharacter::Injury()
{
	if (CanInjury())
	{
		switch (WeaponType)
		{
		case EWeaponType::MELEE:
			MeleeInjury();
			break;
		case EWeaponType::SWORD:
			if (PlayerBehavior != EPlayerBehavior::DEFENSE)
			{
				SwordInjury();
			}
			else
			{
				// UE_LOG(LogTemp,Warning,TEXT("Defense"));
				DefenseInjury();
			}
			break;
		}
	}
}

void ASoulPlayerCharacter::MeleeInjury()
{
	bIgnoreHit = true;
	PlayerBehavior = EPlayerBehavior::INJURY;
	CameraShakeFeedBack(true);
	RandomAnimMontage(MeleeInjuryAnimArray);
	if (PlayerHP <= 10)
	{
		PlayerHP -= 10;
		PlayerHP = 0;
		Die();
	}
	else
	{
		PlayerHP -= 10;
	}
}

void ASoulPlayerCharacter::SwordInjury()
{
	bIgnoreHit = true;
	PlayerBehavior = EPlayerBehavior::INJURY;
	CameraShakeFeedBack(true);
	RandomAnimMontage(SwordInjuryAnimArray);
	if (PlayerHP <= 10)
	{
		PlayerHP -= 10;
		PlayerHP = 0;
		Die();
	}
	else
	{
		PlayerHP -= 10;
	}
}

void ASoulPlayerCharacter::DefenseInjury()
{
	bIgnoreHit = true;
	//这里要设置不能碰撞，不然会造成伤害
	EnableSwordCollision(false);
	CameraShakeFeedBack(false);
	RandomAnimMontage(DefenseInjuryAnimArray);
	if (Stamina <= 5)
	{
		Stamina -= 5;
		Stamina = 0;
	}
	else
	{
		Stamina -= 5;
	}
}

bool ASoulPlayerCharacter::CanInjury()
{
	if (!bIgnoreHit)
	{
		return true;
	}
	return false;
}

void ASoulPlayerCharacter::SwitchLockView()
{
	if(isLock)
	{
		isLock=false;
	}
	else
	{
		isLock=true;
	}
}

void ASoulPlayerCharacter::ViewChange()
{
	if (EnemyTarget)
	{
		FVector PlayerLocation = GetActorLocation();
		FVector TargetLocation = EnemyTarget->GetActorLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
		if (ASoulPlayerController* PlayerController = Cast<ASoulPlayerController>(GetController()))
		{
			PlayerController->SetControlRotation(NewRotation);
			FollowCamera->SetRelativeLocation(FVector(0,0,200));
			FollowCamera->SetRelativeRotation(FRotator(-30,0,0));
		}
	}
}

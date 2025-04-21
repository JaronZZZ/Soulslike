// Fill out your copyright notice in the Description page of Project Settings.


#include "Soulslike/Character/SoulBaseCharacter.h"

#include "Soulslike/AIEnemy/EnemyAIController.h"
#include "SoulPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Soulslike/AIEnemy/BaseEnemy.h"
#include "Soulslike/Logic/SoulEventManager.h"

// Sets default values
ASoulBaseCharacter::ASoulBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	PlayerPosArrow=CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerPosition"));
	PlayerPosArrow->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraBoom->bUsePawnControlRotation = false;

	//奔跑判断变量
	bRunning = false;

	//装备判断变量,此处用于武器
	bEquip = false;

	//是否死亡判断
	bDie = false;

	//是否免疫打击
	bIgnoreHit = false;

	WeaponType = EWeaponType::MELEE;
	MeleeType = EMeleeType::COMMON;
	PlayerBehavior = EPlayerBehavior::IDLE;

	MaxPlayerHP = 100.0f;
	PlayerHP = MaxPlayerHP;
	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	MeleeAttackSubStamina = 10.0f;
	SwordAttackSubStamina = 15.0f;
	MeleeRollingSubStamina = 15.0f;
	SwordRollingSubStamina = 15.0f;
	IncreaseStamina = 10.0f;
	DesiredRotation = FRotator(0, 0, 0);

	RollingForwardValue = 0;
	RollingRightValue = 0;
	RollingAnimPlayRate = 1.0f;

	LastMeleeIndex = 0;
	LastSwordIndex = 0;

	EnemyTarget = nullptr;

	MeleeSphereCollisionRadius = 70.f;
}

// Called when the game starts or when spawned
void ASoulBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoulBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DesiredRotation = CalculateRotation();
}


void ASoulBaseCharacter::FindEnemyPtr()
{
	EnemyTarget = Cast<ABaseEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), InsEnemy));
}

void ASoulBaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	//MovementVector是以角色朝向为基本方向的，换句话说，输入逻辑是对于角色来说的前后左右

	//直接这样给方向会导致按下某个方向以后，只要没有再按方向键，翻滚会一直往这个方向进行
	//如果要给一个解决方法，可以考虑一个TPair<int32,int32>的数据类型
	//每次tick的时候Value++，在Move函数里面设置一个阈值和key判断条件，key不一样就覆盖，然后Value置0，Value超过阈值就全置0
	//不过这样稍微有点拖累性能了，又想到还是在每次播片以后手动置0会好一点
	RollingForwardValue = MovementVector.Y;
	RollingRightValue = MovementVector.X;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		//以下是移动功能实现逻辑
		const FRotator Rotation = PC->GetControlRotation();

		//为了理解这里方向到映射的关系，做以下解释
		//首先创建的FRotationMatrix是一个方向矩阵，此处是只有偏航角方向有变化的坐标系矩阵
		//上述矩阵虽然只在偏航角有变化，但依然具备三个方向的向量
		//为了得到前向向量，按照UE空间轴向逻辑（EAxis），取X轴分量
		//这样就得到了角色前向的单位向量，用于表示方向
		const FVector ForwardDirection = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::Y);


		//得到方向以后会把这个方向与按键映射进行绑定
		//后面的MovementVector其实就是玩家输入信号转化的数字信号
		//这个数字信号按照手柄或者键盘布局，从符合逻辑上来讲，Y轴是上下的，也就是前后，X轴是左右，这样符合玩家直觉
		//将前向绑定在Y轴输入上，正值就显然是前进，负值就是后退
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASoulBaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ASoulBaseCharacter::EnableMeleeCollision()
{
	FVector SpherePos = GetActorLocation() + GetActorForwardVector() * 70.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery3);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	TArray<AActor*> OverlapActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpherePos, MeleeSphereCollisionRadius, ObjectType, nullptr, IgnoreActors, OverlapActors);

	for (auto MeleeAttackEnemy : OverlapActors)
	{
		if (ABaseEnemy* AttackTarget = Cast<ABaseEnemy>(MeleeAttackEnemy))
		{
			AttackTarget->Injury();
		}
	}
}

void ASoulBaseCharacter::RandomAttack(TArray<UAnimMontage*> NeedRandomAnims, int32& LastIndex)
{
	int32 AttackAnimIndex = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		if (AttackAnimIndex != LastIndex)
		{
			LastIndex = AttackAnimIndex;
			CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimIndex]);
		}
		else
		{
			if (AttackAnimIndex == 0)
			{
				int32 AddIndexNum = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
				AttackAnimIndex += AddIndexNum;
				LastIndex = AttackAnimIndex;
				CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimIndex]);
			}
			else
			{
				AttackAnimIndex--;
				LastIndex = AttackAnimIndex;
				CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimIndex]);
			}
		}
	}
}

void ASoulBaseCharacter::RandomAnimMontage(TArray<UAnimMontage*> NeedRandomAnims)
{
	int32 AttackAnimIndex = UKismetMathLibrary::RandomIntegerInRange(0, NeedRandomAnims.Num() - 1);
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		CurAnimIns->Montage_Play(NeedRandomAnims[AttackAnimIndex]);
	}
}

void ASoulBaseCharacter::PlayRollingOrSlideMontage(TArray<UAnimMontage*> RollingOrSlideAnimArray)
{
	if (UAnimInstance* CurAnimIns = GetMesh()->GetAnimInstance())
	{
		if (RollingForwardValue == 1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnimArray[0], RollingAnimPlayRate);
		}
		else if (RollingForwardValue == -1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnimArray[1], RollingAnimPlayRate);
		}
		else if (RollingRightValue == 1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnimArray[2], RollingAnimPlayRate);
		}
		else if (RollingRightValue == -1)
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnimArray[3], RollingAnimPlayRate);
		}
		else
		{
			CurAnimIns->Montage_Play(RollingOrSlideAnimArray[0], RollingAnimPlayRate);
		}
		//这样置0以后又发现，在翻滚可以取消的时候，会出现松开方向，但是可以连续朝同一方向翻滚两次的现象
		//如果需要进一步修正，应该把两个变量暴露给蓝图，然后在通知RollingRest事件之后手动置0
		RollingForwardValue = 0;
		RollingRightValue = 0;
	}
}

void ASoulBaseCharacter::Run()
{
	if (CanRun())
	{
		bRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

bool ASoulBaseCharacter::CanRun()
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

void ASoulBaseCharacter::StopRun()
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ASoulBaseCharacter::Die()
{
	bDie = true;
	EnemyTarget->EnemyAIController->GetBlackboardComponent()->SetValueAsBool("IsAnyoneDie", true);
	RemovePlayerInput();
	//关闭胶囊体碰撞
	if (GetCapsuleComponent())
	{
		// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ASoulBaseCharacter::DeathUI()
{
	USoulEventManager::GetInstancePtr()->OpenFightResultUI.ExecuteIfBound(false);
}

void ASoulBaseCharacter::RemovePlayerInput()
{
	if (ASoulPlayerController* PC = Cast<ASoulPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PlayerMappingContext);
		}
	}
}

float ASoulBaseCharacter::GetCurHp()
{
	return PlayerHP;
}

float ASoulBaseCharacter::GetMaxHP()
{
	return MaxPlayerHP;
}

float ASoulBaseCharacter::GetCurStamina()
{
	return Stamina;
}

float ASoulBaseCharacter::GetMaxStamina()
{
	return MaxStamina;
}

FRotator ASoulBaseCharacter::CalculateRotation()
{
	FVector LastVector = GetCharacterMovement()->GetLastInputVector();

	if (!LastVector.IsZero())
	{
		return UKismetMathLibrary::MakeRotFromX(LastVector);
	}
	else
	{
		return DesiredRotation;
	}
}

void ASoulBaseCharacter::RInterpRotation()
{
	FRotator RInterpRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
	SetActorRotation(FRotator(0, RInterpRot.Yaw, 0));
}

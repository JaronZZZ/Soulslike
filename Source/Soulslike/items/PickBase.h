// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Soulslike/SoulEnumType.h"
#include "PickBase.generated.h"


UCLASS()
class SOULSLIKE_API APickBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//静态网格体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items)
	UStaticMeshComponent* PickItemMesh;

	//骨骼体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items)
	USkeletalMesh* SK_PickItem;

	//服装类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items)
	EClothType ClothType;

	//拾取特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items)
	UNiagaraComponent* PickNia;

	//碰撞体
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items)
	USphereComponent* PickColl;

	//拾取物品到玩家身上
	UFUNCTION()
	void PickItemToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                      const FHitResult& SweepResult);
};

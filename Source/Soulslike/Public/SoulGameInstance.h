// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoulGameInstance.generated.h"

/**
 * 
 */
class USkeletalMesh;

UCLASS()
class SOULSLIKE_API USoulGameInstance : public UGameInstance
{
	GENERATED_BODY()

	USoulGameInstance();

public:
	UPROPERTY()
	USkeletalMesh* TopCloth;

	UPROPERTY()
	USkeletalMesh* BotCloth;

	UPROPERTY()
	USkeletalMesh* Shoes;

	UPROPERTY()
	USkeletalMesh* Hat;

	UPROPERTY()
	bool IsInFightMap;

	UPROPERTY(BlueprintReadWrite)
	bool IsNeedPlayUIAnim;
};

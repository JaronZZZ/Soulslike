// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulEnumType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	NONE,	
	MELEE,		//拳
	SWORD,		//刀
};

UENUM(BlueprintType)
enum class EMeleeType :uint8
{
	NONE,	
	COMMON,			//拳法状态
	PREPAREWAR,		//备战状态
};

UENUM(BlueprintType)
enum class EPlayerBehavior :uint8
{
	NONE,	
	IDLE,			//空闲状态
	ATTACK,			//攻击状态
	ROLLING,		//翻滚
	SLIDE,			//滑步
	DEFENSE,		//防御
	INJURY,			//受击
};


UENUM(BlueprintType)
enum class EClothType :uint8
{
	NONE,
	HAT,			//帽子
	TOPCLOTH,		//上衣
	BOTCLOTH,		//裤子
	SHOES,			//鞋子
};

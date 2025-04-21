// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soulslike/UI/SoulWidgetRule.h"
#include "SoulFightMainUI.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class SOULSLIKE_API USoulFightMainUI : public USoulWidgetRule
{
	GENERATED_BODY()

public:

	//血量文本
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* Text_PlayerHP;

	//体力文本
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* Text_Stamina;
	
	//血条
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UProgressBar* PlayerHPBar;

	//体力条
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UProgressBar* StaminaBar;


};

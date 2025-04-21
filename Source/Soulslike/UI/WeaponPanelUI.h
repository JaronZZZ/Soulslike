// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulWidgetRule.h"
#include "Soulslike/SoulEnumType.h"
#include "WeaponPanelUI.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UWeaponPanelUI : public USoulWidgetRule
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponText(EWeaponType WT);

	
};

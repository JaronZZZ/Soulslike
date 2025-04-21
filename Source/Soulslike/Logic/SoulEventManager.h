// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soulslike/SoulEnumType.h"
#include "SoulEventManager.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOpenFightResultUI, bool)
DECLARE_DELEGATE_OneParam(FUpdateWeaponText,EWeaponType)

UCLASS()
class SOULSLIKE_API USoulEventManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SoulEventManager")
	static USoulEventManager* GetInstancePtr();

private:
	static USoulEventManager* s_Instance;

public:
	FOpenFightResultUI OpenFightResultUI;

	FUpdateWeaponText UpdateWeaponText;
};

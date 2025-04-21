// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulWidgetRule.h"
#include "SoulFightResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API USoulFightResultWidget : public USoulWidgetRule
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	int32 CountDownTime;
	float WaitTime;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int32 GetCountDownTime();

	UFUNCTION(BlueprintImplementableEvent)
	void SetCountDownText();

	UFUNCTION(BlueprintImplementableEvent)
	void SetResultText(bool IsWin);
};

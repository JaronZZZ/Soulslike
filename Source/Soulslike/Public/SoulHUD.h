// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Soulslike/UI/MainMapUI.h"
#include "Soulslike/UI/SoulFightMainUI.h"
#include "Soulslike/UI/SoulFightResultWidget.h"
#include "SoulHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API ASoulHUD : public AHUD
{
	GENERATED_BODY()

	ASoulHUD();

	virtual void BeginPlay() override;

	//用以承接获取到的UI类，此处是类型安全容器，比UClass* 安全，编译器和运行时检查类型匹配
	TSubclassOf<USoulFightMainUI> FightMainUIClass;
	TSubclassOf<USoulFightResultWidget> FightResultUIClass;
	TSubclassOf<UMainMapUI> MainMapUIClass;

	//实际在GameMode下面实现的UI
	USoulFightMainUI* FightMainUI;
	USoulFightResultWidget* FightResultUI;
	UMainMapUI* MainMapUI;

public:
	//UI创建
	//开始战斗时的UI
	void CreateFightMainUI();

	//战斗结果
	void CreateFightResultUI(bool IsWinww);

	//主场景UI(没有敌人的那个)
	void CreateMainMapUI();
};

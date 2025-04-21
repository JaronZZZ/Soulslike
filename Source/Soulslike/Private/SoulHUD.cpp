// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulHUD.h"

#include "SoulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Soulslike/Character/SoulPlayerCharacter.h"
#include "Soulslike/UI/SoulFightResultWidget.h"
#include "Soulslike/Logic/SoulEventManager.h"
#include "Soulslike/UI/SoulFightMainUI.h"

ASoulHUD::ASoulHUD()
{
	static ConstructorHelpers::FClassFinder<USoulFightMainUI> UI_FightMainUIClass(TEXT("/Game/_MyGame/BP/UI/BPUI_FightMain"));
	static ConstructorHelpers::FClassFinder<USoulFightResultWidget> UI_FightResultUIClass(TEXT("/Game/_MyGame/BP/UI/BPUI_FightResult"));
	static ConstructorHelpers::FClassFinder<UMainMapUI> UI_MainMapUIClass(TEXT("/Game/_MyGame/BP/UI/BPUI_MainMap"));

	FightMainUIClass = UI_FightMainUIClass.Class;
	FightResultUIClass = UI_FightResultUIClass.Class;
	MainMapUIClass = UI_MainMapUIClass.Class;
}

void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();
	USoulEventManager::GetInstancePtr()->OpenFightResultUI.BindUObject(this, &ASoulHUD::CreateFightResultUI);
	if(USoulGameInstance* GameInstance=Cast<USoulGameInstance>(GetGameInstance()))
	{
		if(GameInstance->IsInFightMap==false)
		{
			CreateMainMapUI();
		}
	}
}

void ASoulHUD::CreateFightMainUI()
{
	FightMainUI = CreateWidget<USoulFightMainUI>(GetWorld(), FightMainUIClass);
	FightMainUI->AddToViewport();
}

void ASoulHUD::CreateFightResultUI(bool IsWin)
{
	FightMainUI->RemoveFromParent();
	FightResultUI = CreateWidget<USoulFightResultWidget>(GetWorld(), FightResultUIClass);
	FightResultUI->SetResultText(IsWin);
	FightResultUI->AddToViewport();
}

void ASoulHUD::CreateMainMapUI()
{
	MainMapUI = CreateWidget<UMainMapUI>(GetWorld(), MainMapUIClass);
	if (ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		USoulEventManager::GetInstancePtr()->UpdateWeaponText.ExecuteIfBound(PlayerCharacter->WeaponType);
	}
	MainMapUI->AddToViewport();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPlayerController.h"

#include "SoulHUD.h"
#include "Soulslike/AIEnemy/BaseEnemy.h"
#include "Soulslike/Character/SoulBaseCharacter.h"

void ASoulPlayerController::OpenFightUI()
{
	if(ASoulBaseCharacter* PlayerCharacter=Cast<ASoulBaseCharacter>(GetPawn()))
	{
		PlayerCharacter->FindEnemyPtr();
	}
	if(ASoulHUD* FightHUD=Cast<ASoulHUD>(GetHUD()))
	{
		FightHUD->CreateFightMainUI();
	}
}

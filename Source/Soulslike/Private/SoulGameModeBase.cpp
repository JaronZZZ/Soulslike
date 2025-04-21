// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameModeBase.h"

#include "SoulHUD.h"
#include "SoulPlayerController.h"
#include "Soulslike/Character/SoulBaseCharacter.h"

ASoulGameModeBase::ASoulGameModeBase()
{

	static ConstructorHelpers::FClassFinder<ASoulBaseCharacter> BPPlayerClass(TEXT("/Game/_MyGame/BP/BP_PlayerCharacter"));
	if(BPPlayerClass.Class) {
		UE_LOG(LogTemp, Warning, TEXT("Success load class: %s"), *BPPlayerClass.Class->GetName());
		DefaultPawnClass = BPPlayerClass.Class;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load BPPlayerClass!"));
	}
	
	
	PlayerControllerClass=ASoulPlayerController::StaticClass();

	HUDClass=ASoulHUD::StaticClass();
}

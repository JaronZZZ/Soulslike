// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPanelUI.h"

#include "Soulslike/Logic/SoulEventManager.h"

void UWeaponPanelUI::NativeConstruct()
{
	Super::NativeConstruct();
	USoulEventManager::GetInstancePtr()->UpdateWeaponText.BindUObject(this,&UWeaponPanelUI::UpdateWeaponText);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulEventManager.h"

USoulEventManager* USoulEventManager::s_Instance=nullptr;

USoulEventManager* USoulEventManager::GetInstancePtr()
{
	if(s_Instance==nullptr)
	{
		s_Instance=NewObject<USoulEventManager>();
		s_Instance->AddToRoot();
	}
	return s_Instance;
}

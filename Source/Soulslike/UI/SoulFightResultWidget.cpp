// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulFightResultWidget.h"

#include "SoulGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USoulFightResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CountDownTime=5;
	WaitTime=0.f;
	SetCountDownText();
	
}

void USoulFightResultWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(CountDownTime>0)
	{
		WaitTime+=InDeltaTime;
		if(WaitTime>=1)
		{
			CountDownTime--;
			SetCountDownText();
			WaitTime=0.f;
		}
		if(CountDownTime==0)
		{
			if(USoulGameInstance* GameInstance=Cast<USoulGameInstance>(GetGameInstance()))
			{
				GameInstance->IsInFightMap=false;
			}
			UGameplayStatics::OpenLevel(GetWorld(),"MainMap");
		}
	}
}

int32 USoulFightResultWidget::GetCountDownTime()
{
	return CountDownTime;
}

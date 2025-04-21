// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeMapTrigger.h"

#include "NiagaraComponent.h"
#include "SoulGameInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChangeMapTrigger::AChangeMapTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxTrigger=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);
	
	Niagara=CreateDefaultSubobject<UNiagaraComponent>(TEXT("TriggerNiagara"));
	Niagara->SetupAttachment(RootComponent);

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this,&AChangeMapTrigger::ChangeMapFunction);

}

// Called when the game starts or when spawned
void AChangeMapTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChangeMapTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChangeMapTrigger::ChangeMapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(USoulGameInstance* GameInstance = Cast<USoulGameInstance>(GetGameInstance()))
	{
		GameInstance->IsInFightMap=true;
	}
	UGameplayStatics::OpenLevel(GetWorld(),"FightMap");
}

 
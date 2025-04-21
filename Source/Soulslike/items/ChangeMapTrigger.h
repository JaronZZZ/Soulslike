// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeMapTrigger.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class SOULSLIKE_API AChangeMapTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangeMapTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Items")
	UBoxComponent* BoxTrigger;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Items")
	UNiagaraComponent* Niagara;

	UFUNCTION()
	void ChangeMapFunction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};

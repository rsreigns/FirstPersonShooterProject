// Fill out your copyright notice in the Description page of Project Settings.


#include "TESTSpawnBox.h"

ATESTSpawnBox::ATESTSpawnBox()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
}

void ATESTSpawnBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATESTSpawnBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


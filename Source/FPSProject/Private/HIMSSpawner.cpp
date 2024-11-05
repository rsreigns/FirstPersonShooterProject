// Fill out your copyright notice in the Description page of Project Settings.


#include "HISMSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstance.h"

#include "FPSProject/DebugHelper.h"

AHISMSpawner::AHISMSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	this->SetActorEnableCollision(false);
	ISMComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMComponent"));
	ISMComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ISMComp->SetVisibility(true);
	ISMComp->SetupAttachment(RootComponent);
	ISMComp->NumCustomDataFloats = 3;
	ISMComp->SetNumCustomDataFloats(3);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Game/Myfiles/Dummy.Dummy'"));
	if (CubeMeshAsset.Succeeded())
	{
		ISMComp->SetStaticMesh(CubeMeshAsset.Object);
	}
	else
	{
		DEBUG::PrintString("Unable to fetch the mesh ", 10.f, FColor::Red);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Myfiles/Material/M_BoxMaterial.M_BoxMaterial'"));
	if (MaterialAsset.Succeeded())
	{
		ISMComp->SetMaterial(0, MaterialAsset.Object);
	}
	else
	{
		DEBUG::PrintString("Unable to fetch the material ", 10.f, FColor::Red);
	}
}



void AHISMSpawner::BeginPlay()
{
	Super::BeginPlay();

	
	
}




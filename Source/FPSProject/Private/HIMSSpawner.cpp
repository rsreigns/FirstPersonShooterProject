// Fill out your copyright notice in the Description page of Project Settings.


#include "HISMSpawner.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstance.h"
#include "BoxToSpawn.h"

#include "FPSProject/DebugHelper.h"

AHISMSpawner::AHISMSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	this->SetActorEnableCollision(false);
	ISMComp = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("ISMComponent"));
	ISMComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ISMComp->SetVisibility(true);
	ISMComp->SetupAttachment(RootComponent);
	ISMComp->NumCustomDataFloats = 3;

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

ABoxToSpawn* AHISMSpawner::GetPoolObject()
{
	if (BoxPool.Num() > 0)
	{
		int32 Num = BoxPool.Num();
		ABoxToSpawn* PooledObject = BoxPool[Num - 1];
		PooledObject->SetActorHiddenInGame(false);
		PooledObject->SetActorEnableCollision(true);
		BoxPool.RemoveAt(Num - 1);
		return PooledObject;
	}
	return nullptr;
}

void AHISMSpawner::ReturnToPool(ABoxToSpawn* Object)
{
	if (Object)
	{
		Object->SetActorHiddenInGame(true);
		Object->SetActorEnableCollision(false);
		Object->SetActorTransform(PoolTransform) ;
		BoxPool.Add(Object);
	}
}

void AHISMSpawner::BeginPlay()
{
	Super::BeginPlay();

	PoolTransform = GetActorTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < MaxPoolSize; i++)
	{
		ABoxToSpawn* PoolObject = GetWorld()->SpawnActor<ABoxToSpawn>(ABoxToSpawn::StaticClass(), PoolTransform, SpawnParams);
		BoxPool.Add(PoolObject);
		PoolObject->SetActorHiddenInGame(false);
		PoolObject->SetActorEnableCollision(false);
	}
	
}




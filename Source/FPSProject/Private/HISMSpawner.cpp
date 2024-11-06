// Fill out your copyright notice in the Description page of Project Settings.


#include "HISMSpawner.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstance.h"
#include "BoxToSpawn.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

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

ABoxToSpawn* AHISMSpawner::GetPoolObject(double X, double Y, double Z,double HealthValue, double ScoreValue, FTransform Transform)
{
		// first spawn in the object
		int32 Num = BoxPool.Num();
		ABoxToSpawn* PooledObject = BoxPool[Num - 1];
		BoxPool.RemoveAt(Num - 1);
		SpawnedPool.Add(PooledObject);

		//Then Set Defaults for pooled Object
		PooledObject->CurrentHealth = HealthValue;
		PooledObject->GivenHealth = HealthValue;
		PooledObject->ScoreToAward = ScoreValue;
		PooledObject->SetActorHiddenInGame(false);
		PooledObject->SetActorEnableCollision(true);
		PooledObject->SetActorTransform(Transform);
		PooledObject->ApplyBoxDefaults();

		//Then Add instance from HISM Component
		int32 InstanceIndex = ISMComp->AddInstance(Transform, true);

		ISMComp->SetCustomDataValue(InstanceIndex, 0, X / 255.0, true);
		ISMComp->SetCustomDataValue(InstanceIndex, 1, Y / 255.0, true);
		ISMComp->SetCustomDataValue(InstanceIndex, 2, Z / 255.0, true);
		
		return PooledObject;

}


void AHISMSpawner::ReturnToPool(ABoxToSpawn* Object)
{
	if (Object)
	{
		Object->SetActorHiddenInGame(true);
		Object->SetActorEnableCollision(false);
		Object->SetActorTransform(PoolTransform);


		int32 InstanceToRemove = SpawnedPool.Find(Object);
		if (InstanceToRemove != INDEX_NONE)
		{
			if (ISMComp->IsValidInstance(InstanceToRemove))
			{
				ISMComp->RemoveInstance(InstanceToRemove);


				int32 LastIndex = SpawnedPool.Num() - 1;
				if (InstanceToRemove != LastIndex)
				{
					SpawnedPool[InstanceToRemove] = SpawnedPool[LastIndex];
				}

				SpawnedPool.RemoveAt(LastIndex);

				BoxPool.Add(Object);
			}
		}
	}
}



void AHISMSpawner::BeginPlay()
{
	Super::BeginPlay();
	PoolTransform = GetActorTransform();

	// Set up initial properties
	ISMComp->ClearInstances();
	RemainingPoolToSpawn = MaxPoolSize; // Keep track of remaining objects to spawn

	// Set up a timer to spawn instances in batches
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this,&ThisClass::SpawnPoolBatch, 0.01f, true);
}

void AHISMSpawner::SpawnPoolBatch()
{
	if (RemainingPoolToSpawn > 0)
	{
		int32 BatchSize = FMath::Min(10, RemainingPoolToSpawn); // Spawn in batches of 10
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (int32 i = 0; i < BatchSize; i++)
		{
			ABoxToSpawn* PoolObject = GetWorld()->SpawnActor<ABoxToSpawn>(ABoxToSpawn::StaticClass(), PoolTransform, SpawnParams);
			BoxPool.Add(PoolObject);
			PoolObject->SetActorHiddenInGame(true);
			PoolObject->SetActorEnableCollision(false);
		}

		RemainingPoolToSpawn -= BatchSize;
	}
	else
	{
		// All objects have been spawned; clear the timer
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}




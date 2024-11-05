// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMSpawner.generated.h"

class ABoxToSpawn;
class UHierarchicalInstancedStaticMeshComponent;


UCLASS()
class FPSPROJECT_API AHISMSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AHISMSpawner();

	UHierarchicalInstancedStaticMeshComponent* ISMComp;
	TArray<int32> InstanceIndices;

	ABoxToSpawn* GetPoolObject();
	void ReturnToPool(ABoxToSpawn* Object);


protected:

	int32 MaxPoolSize = 1000;
	TArray<ABoxToSpawn*> BoxPool;
	FTransform PoolTransform;

	virtual void BeginPlay() override;



};

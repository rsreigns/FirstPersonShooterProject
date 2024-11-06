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

	ABoxToSpawn* GetPoolObject(double X, double Y, double Z,  double HealthValue, double ScoreValue, FTransform Transform);
	void ReturnToPool(ABoxToSpawn* Object);


protected:

	int32 MaxPoolSize = 1000;
	UPROPERTY()
	TArray<ABoxToSpawn*> BoxPool;

	TArray<ABoxToSpawn*> SpawnedPool;
	FTransform PoolTransform;

	virtual void BeginPlay() override;


	UFUNCTION()
	void SpawnPoolBatch();

	FTimerHandle SpawnTimerHandle;
	int32 RemainingPoolToSpawn;

};

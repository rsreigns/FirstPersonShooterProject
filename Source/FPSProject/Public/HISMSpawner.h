// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HISMSpawner.generated.h"

class UInstancedStaticMeshComponent;
class UHierarchicalInstancedStaticMeshComponent;

UCLASS()
class FPSPROJECT_API AHISMSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AHISMSpawner();

	UHierarchicalInstancedStaticMeshComponent* ISMComp;
	TArray<int32> InstanceIndices;


protected:

	virtual void BeginPlay() override;



};

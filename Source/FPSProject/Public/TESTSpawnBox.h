// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TESTSpawnBox.generated.h"

class UStaticMeshComponent;

UCLASS()
class FPSPROJECT_API ATESTSpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	

	ATESTSpawnBox();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Core|Mesh")
	UStaticMeshComponent* StaticMeshComponent;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxToSpawn.generated.h"

class UMaterial;
class UStaticMeshComponent;

UCLASS()
class FPSPROJECT_API ABoxToSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoxToSpawn();

protected:

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	double CurrentHealth;
public:	
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category = "MeshComp")
	UStaticMeshComponent* MeshComp;
#pragma endregion
	UPROPERTY(EditInstanceOnly)
	FString BoxName;
	UPROPERTY(EditInstanceOnly)
	double GivenHealth;
	UPROPERTY(EditInstanceOnly)
	double ScoreToAward;
	UPROPERTY(EditInstanceOnly)
	int32 ColorX; 
	UPROPERTY(EditInstanceOnly)
	int32 ColorY;
	UPROPERTY(EditInstanceOnly)
	int32 ColorZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Material")
	UMaterial* MaterialToApply;


	virtual void Tick(float DeltaTime) override;

};

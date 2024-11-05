// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxToSpawn.generated.h"


class UHierarchicalInstancedStaticMeshComponent;
class UWidgetComponent;
class UParticleSystem;
class UBoxHealthWidget;
class USoundBase;
class UBoxComponent;
class AHISMSpawner;

UCLASS()
class FPSPROJECT_API ABoxToSpawn : public AActor
{
	GENERATED_BODY()
	
private:
	int32 InstanceIndex;
	UHierarchicalInstancedStaticMeshComponent* ISMCompRef;
public:	
	ABoxToSpawn();

protected:

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core")
	UBoxComponent* BoxComponent;

#pragma endregion
	UPROPERTY(EditInstanceOnly)
	FString BoxName;
	UPROPERTY(EditInstanceOnly)
	double GivenHealth;
	UPROPERTY(EditInstanceOnly)
	double ScoreToAward;


	void ApplyDefaults(double X, double Y, double Z, double HealthValue, double ScoreValue, FTransform Transform,
		UHierarchicalInstancedStaticMeshComponent* Component,int32 Index);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Particle")
	UParticleSystem* ParticleSystem;

	USoundBase* ExplosionSound;

	UPROPERTY()
	double CurrentHealth;

};

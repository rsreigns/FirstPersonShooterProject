// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxToSpawn.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UParticleSystem;
class USoundBase;
class UBoxComponent;

UCLASS()
class FPSPROJECT_API ABoxToSpawn : public AActor
{
	GENERATED_BODY()
	
public:
	UHierarchicalInstancedStaticMeshComponent* ISMCompRef;
	ABoxToSpawn();

protected:
	virtual void BeginPlay();
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


	void ApplyBoxDefaults();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Particle")
	TSoftObjectPtr<UParticleSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Sound")
	TSoftObjectPtr<USoundBase> ExplosionSound;

	UPROPERTY()
	double CurrentHealth;

	void LoadEmitterAndSoundAsync();
	void OnEmitterAndSoundLoaded();

	USoundBase* LoadedExplosionSound;
	UParticleSystem* LoadedExplosionEffect;

};

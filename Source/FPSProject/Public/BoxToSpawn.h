// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxToSpawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

class UMaterial;
class UStaticMeshComponent;
//class UHierarchicalInstancedStaticMeshComponent;
class UInstancedStaticMeshComponent;
class UWidgetComponent;
class UParticleSystem;
class UUserWidget;
class UBoxHealthWidget;
class USoundBase;
class UBoxComponent;

UCLASS()
class FPSPROJECT_API ABoxToSpawn : public AActor
{
	GENERATED_BODY()
	
private:
	int32 InstanceIndex;
	UInstancedStaticMeshComponent* ISMCompRef;
public:	
	ABoxToSpawn();

protected:

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
#pragma region Components

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Core|Widget")
	UWidgetComponent* WidgetComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Widget")
	UBoxComponent* BoxComponent;

#pragma endregion
	UPROPERTY(EditInstanceOnly)
	FString BoxName;
	UPROPERTY(EditInstanceOnly)
	double GivenHealth;
	UPROPERTY(EditInstanceOnly)
	double ScoreToAward;

	UPROPERTY(BlueprintAssignable,Category="Event")
	FOnHealthChanged OnHealthChanged;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Material")
	UMaterial* MaterialToApply;

	void ApplyDefaults(double X, double Y, double Z, double HealthValue, double ScoreValue, FTransform Transform,
		UInstancedStaticMeshComponent* Component,int32 Index);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Particle")
	UParticleSystem* ParticleSystem;

	USoundBase* ExplosionSound;

	UPROPERTY()
	double CurrentHealth;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float, NewScore);

class UHierarchicalInstancedStaticMeshComponent;
class AHISMSpawner;

USTRUCT(BlueprintType)
struct FJSONObjectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintreadOnly)
	double Health;

	UPROPERTY(BlueprintReadOnly)
	double Score;

	UPROPERTY(BlueprintReadOnly)
	double ColorR;
	UPROPERTY(BlueprintReadOnly)
	double ColorG;
	UPROPERTY(BlueprintReadOnly)
	double ColorB;


};

UCLASS()
class FPSPROJECT_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float PlayerScore = 0.f;

public:
	UFUNCTION(BlueprintCallable)
	void RetrieveDataFromJSON();

	UPROPERTY(BlueprintReadOnly)
	TArray<FJSONObjectData> JSONObjectData;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnScoreChanged OnScoreChanged;

	void AddPlayerScore(float ScoreToAdd);

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Core|Spawn")
	AHISMSpawner* HISMObject;

};

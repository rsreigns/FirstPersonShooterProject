// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSProjectGameMode.generated.h"

//USTRUCT(BlueprintType)
//struct FJSONObjectDataa
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadOnly)
//	FString Name;
//
//	UPROPERTY(BlueprintreadOnly)
//	double Health;
//
//	UPROPERTY(BlueprintReadOnly)
//	double Score;
//
//	UPROPERTY(BlueprintReadOnly)
//	int32 ColorR;
//	UPROPERTY(BlueprintReadOnly)
//	int32 ColorG;
//	UPROPERTY(BlueprintReadOnly)
//	int32 ColorB;
//
//
//};

UCLASS(minimalapi)
class AFPSProjectGameMode : public AGameModeBase 
{
	GENERATED_BODY()

public:
	AFPSProjectGameMode();

	//void RetrieveDataFromJSON();

	//UPROPERTY(BlueprintReadOnly)
	//TArray<FJSONObjectData> JSONObjectData;

	//virtual void PostLogin(APlayerController* NewPlayer) override;

};




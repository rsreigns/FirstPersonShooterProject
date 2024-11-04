// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UMyUserScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateScore(float NewScore);

	UPROPERTY(BlueprintReadOnly)
	float CurrentScore;
	
};

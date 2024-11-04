// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UUserWidget;
class UMyUserScoreWidget;

UCLASS()
class FPSPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly,Category = "Core|UI")
	TArray<TSubclassOf<UUserWidget>> WidgetsToAdd;

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateScore(float NewScore);

	UMyUserScoreWidget* ScoreWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|UI|Score")
	TSubclassOf<UMyUserScoreWidget> ScoreWidgetClass;
	
};

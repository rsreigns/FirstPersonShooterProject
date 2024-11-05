// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UUserWidget;
class UMyUserScoreWidget;
class UBoxHealthWidget;

UCLASS()
class FPSPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:


	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateScore(float NewScore);

	UMyUserScoreWidget* ScoreWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|UI|Score")
	TSubclassOf<UMyUserScoreWidget> ScoreWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category= "Core|UI")
	TArray<TSubclassOf<UUserWidget>> WidgetsToAdd;

	UPROPERTY(EditDefaultsOnly, Category = "Core|UI")
	TSubclassOf<UBoxHealthWidget> BoxHealthWidget;
	UBoxHealthWidget* HealthWidget;
	
};

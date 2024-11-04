// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoxHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBoxHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void UpdateHealth(float NewHealth);

	UPROPERTY(BlueprintReadOnly,Category="Native|UpdatedHealth")
	float Health;

	bool bFirstUpdate = true;

	UPROPERTY(BlueprintReadOnly, Category = "Native|UpdatedHealth")
	float MaxHealth;
};

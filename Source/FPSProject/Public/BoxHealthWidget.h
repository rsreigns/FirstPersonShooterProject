// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoxHealthWidget.generated.h"

class ABoxToSpawn;

UCLASS()
class FPSPROJECT_API UBoxHealthWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	FTimerHandle WidgetHandle;
public:

	void HideWidget();
	void SeeWidget(ABoxToSpawn* Object);

	UPROPERTY(BlueprintReadOnly, Category = "Native|UpdatedHealth")
	float Percent;
	UPROPERTY(BlueprintReadOnly, Category = "Native|UpdatedHealth")
	float CurrentHP;


};

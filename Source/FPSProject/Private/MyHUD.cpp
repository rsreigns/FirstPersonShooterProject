// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"


void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UUserWidget>& Widget : WidgetsToAdd)
	{
		if (!Widget)continue;
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(),Widget);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}

	}
}

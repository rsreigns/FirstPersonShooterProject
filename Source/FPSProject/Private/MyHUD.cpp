// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "MyUserScoreWidget.h"
#include "FPSGameMode.h"

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

	ScoreWidget = CreateWidget<UMyUserScoreWidget>(GetWorld(), ScoreWidgetClass);
	if (ScoreWidget)
	{
		ScoreWidget->AddToViewport();
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnScoreChanged.AddDynamic(this, &ThisClass::UpdateScore);
		}
	}

}

void AMyHUD::UpdateScore(float NewScore)
{
	if (ScoreWidget)
	{
		ScoreWidget->UpdateScore(NewScore);
	}
}

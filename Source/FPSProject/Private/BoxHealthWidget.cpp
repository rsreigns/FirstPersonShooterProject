// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxHealthWidget.h"
#include "BoxToSpawn.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "FPSProject/DebugHelper.h"




void UBoxHealthWidget::HideWidget()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UBoxHealthWidget::SeeWidget(ABoxToSpawn* Object)
{
	CurrentHP = Object->CurrentHealth ;
    Percent = 1 - ( CurrentHP / Object->GivenHealth);
	if (GetWorld())
	{
		bool IsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(WidgetHandle);
		if (IsTimerActive)
		{
			GetWorld()->GetTimerManager().ClearTimer(WidgetHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(WidgetHandle, this, &ThisClass::HideWidget, 0.4f, false, -1.f);
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}

}

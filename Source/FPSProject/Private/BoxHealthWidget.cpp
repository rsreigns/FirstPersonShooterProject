// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxHealthWidget.h"
#include "BoxToSpawn.h"

#include "FPSProject/DebugHelper.h"

void UBoxHealthWidget::UpdateHealth(float NewHealth)
{
	// code for changing progress bar dynamically
	
	if (bFirstUpdate)
	{
		MaxHealth = NewHealth;
		bFirstUpdate = false;
	}
		
	Health = NewHealth;
	/*DEBUG::PrintString(FString::Printf(TEXT("Max Health :%f and Health = %f"), MaxHealth,Health));*/
}

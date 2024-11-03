// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxToSpawn.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

#include "FPSProject/DebugHelper.h"
ABoxToSpawn::ABoxToSpawn()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Script/Engine.StaticMesh'/Game/Myfiles/Dummy.Dummy'"));
	if (CubeMeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(CubeMeshAsset.Object);
	}
	else
	{
		DEBUG::PrintString("Unable to fetch the mesh ");
	}

}

void ABoxToSpawn::BeginPlay()
{
	Super::BeginPlay();
	if (MaterialToApply)
	{
		UMaterialInstanceDynamic* DynamicMaterial = MeshComp->CreateDynamicMaterialInstance(0, MaterialToApply);
		DynamicMaterial->SetScalarParameterValue("Red", ColorX / 255);
		DynamicMaterial->SetScalarParameterValue("Green", ColorY / 255);
		DynamicMaterial->SetScalarParameterValue("Blue", ColorZ / 255);
		MeshComp->SetMaterial(0, DynamicMaterial);
	}
	CurrentHealth = GivenHealth;
}

float ABoxToSpawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GivenHealth - DamageAmount > 0)
	{
		GivenHealth -= DamageAmount;
		DEBUG::PrintString(FString::Printf(TEXT("CurrentHealth of %s :%f "), *BoxName, GivenHealth));
		return DamageAmount;
	}
	else
	{
		DEBUG::PrintString(FString::Printf(TEXT("Destroyed, Current Health  :%f "), GivenHealth));
		//add score
		//add effects
		Destroy(); // may send to pool, if implemented
		return DamageAmount;
	}

}







// Called every frame
void ABoxToSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Game/Myfiles/Dummy.Dummy'"));
	if (CubeMeshAsset.Succeeded())                 
	{
		DEBUG::PrintString("Mesh fetched successfully ");
		MeshComp->SetStaticMesh(CubeMeshAsset.Object);
	}
	else
	{
		DEBUG::PrintString("Unable to fetch the mesh ");
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/Myfiles/Material/M_BoxMaterial.M_BoxMaterial'"));
	if (MaterialAsset.Succeeded())
	{
		DEBUG::PrintString("Material fetched successfully ");
		MaterialToApply = MaterialAsset.Object;
		MeshComp->SetMaterial(0,MaterialAsset.Object);
	}
	else
	{
		DEBUG::PrintString("Unable to fetch the material ", 10.f);
	}

}

void ABoxToSpawn::BeginPlay()
{
	Super::BeginPlay();


	CurrentHealth = GivenHealth;
}

void ABoxToSpawn::ApplyMaterialToBox(double X, double Y , double Z)
{
	if (MaterialToApply)
	{
		UMaterialInstanceDynamic* DynamicMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue("Red", X / 255.0f);
			DynamicMaterial->SetScalarParameterValue("Green", Y / 255.0f);
			DynamicMaterial->SetScalarParameterValue("Blue", Z / 255.0f);

			MeshComp->SetMaterial(0, DynamicMaterial);

		}
		else
		{
			DEBUG::PrintString("Failed to create dynamic material instance.", 10.f);
		}
	}
	else
	{
		DEBUG::PrintString("MaterialToApply is null.", 10.f);
	}

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
		DEBUG::PrintString(FString::Printf(TEXT("Destroyed, Current Health  :%f "), CurrentHealth));
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


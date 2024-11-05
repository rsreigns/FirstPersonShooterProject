// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxToSpawn.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "BoxHealthWidget.h"
#include "Particles/ParticleSystem.h"
#include "Sound/Soundbase.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "HISMSpawner.h"
#include "Components/BoxComponent.h"



#include "FPSProject/DebugHelper.h"
ABoxToSpawn::ABoxToSpawn()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent")); 
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxComponent->SetHiddenInGame(false, true);
	BoxComponent->SetBoxExtent(FVector(64.f, 64.f, 64.f));


	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Myfiles/P_Explosion.P_Explosion'"));
	if (ParticleAsset.Succeeded())
	{
		ParticleSystem = ParticleAsset.Object; 
	}
	else
	{
		DEBUG::PrintString("Failed to load particle system", 10.f, FColor::Red); 
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("SoundWave'/Game/Myfiles/Sound/Explosion02.Explosion02'"));
	if (SoundAsset.Succeeded())
	{
		ExplosionSound = SoundAsset.Object;
	}
	else
	{
		DEBUG::PrintString("Failed to Explosion sound", 10.f, FColor::Red);
	}
}

void ABoxToSpawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxToSpawn::ApplyDefaults(double X, double Y , double Z,double HealthValue, double ScoreValue, FTransform Transform,
	UHierarchicalInstancedStaticMeshComponent* Component, int32 Index)
{
	ISMCompRef = Component;
	BoxComponent->SetWorldTransform(Transform);
	BoxComponent->ComponentTags.Add("HitBox");

	InstanceIndex = Index;
	ISMCompRef->AddInstance(Transform, true);

	ISMCompRef->SetCustomDataValue(InstanceIndex, 0, X/255.0, true);
	ISMCompRef->SetCustomDataValue(InstanceIndex, 1, Y/255.0, true);
	ISMCompRef->SetCustomDataValue(InstanceIndex, 2, Z/255.0, true);
	GivenHealth = HealthValue;
	CurrentHealth = GivenHealth;
	ScoreToAward = ScoreValue;
}



float ABoxToSpawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth - DamageAmount > 0)
	{
		return DamageAmount;
	}
	else
	{
		DEBUG::PrintString(FString::Printf(TEXT("Removed Instance : %d"), InstanceIndex), 5.f,FColor::Red);

		if (ParticleSystem)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, FTransform(GetActorRotation(), GetActorLocation(), FVector(1.f, 1.f, 1.f)),true,EPSCPoolMethod::AutoRelease);
		}
		if (ExplosionSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ExplosionSound, GetActorLocation());
		}
		
		AFPSGameMode* GM = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM)
		{
			GM->AddPlayerScore(ScoreToAward);
			GM->HISMObject->ReturnToPool(this);
			if (ISMCompRef && ISMCompRef->IsValidInstance(InstanceIndex))
			{
				ISMCompRef->RemoveInstance(InstanceIndex);
			}
			else 
			{
				DEBUG::PrintString(FString::Printf(TEXT("Invalid Instance : %f"),InstanceIndex), 3.f, FColor::Red);
			}
		}
		return DamageAmount;
	}

}




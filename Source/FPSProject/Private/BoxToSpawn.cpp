// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxToSpawn.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/Soundbase.h"
#include "HISMSpawner.h"
#include "Components/BoxComponent.h"



#include "FPSProject/DebugHelper.h"


ABoxToSpawn::ABoxToSpawn()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent")); 



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

void ABoxToSpawn::ApplyBoxDefaults()
{
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxComponent->SetHiddenInGame(true, true);
	BoxComponent->SetBoxExtent(FVector(64.f, 64.f, 64.f));  
	BoxComponent->ComponentTags.Add("HitBox");
}

float ABoxToSpawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth > 0)
	{
		return DamageAmount;
	}

	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, FTransform(GetActorRotation(),
			GetActorLocation(), FVector(1.f, 1.f, 1.f)),true,EPSCPoolMethod::AutoRelease);
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
	}
	return DamageAmount;

}




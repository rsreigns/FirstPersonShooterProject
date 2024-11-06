// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxToSpawn.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/Soundbase.h"
#include "HISMSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"




#include "FPSProject/DebugHelper.h"


ABoxToSpawn::ABoxToSpawn()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent")); 



	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Myfiles/P_Explosion.P_Explosion'"));
	//if (ParticleAsset.Succeeded())
	//{
	//	ExplosionEffect = ParticleAsset.Object;
	//}
	//else
	//{
	//	DEBUG::PrintString("Failed to load particle system", 10.f, FColor::Red); 
	//}
	//static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("SoundWave'/Game/Myfiles/Sound/Explosion02.Explosion02'"));
	//if (SoundAsset.Succeeded())
	//{
	//	ExplosionSound = SoundAsset.Object;
	//}
	//else
	//{
	//	DEBUG::PrintString("Failed to Explosion sound", 10.f, FColor::Red);
	//}
}
void ABoxToSpawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxToSpawn::LoadEmitterAndSoundAsync()
{
	ExplosionEffect = TSoftObjectPtr<UParticleSystem>(FSoftObjectPath("/Game/Myfiles/P_Explosion.P_Explosion"));
	ExplosionSound = TSoftObjectPtr<USoundBase>(FSoftObjectPath("/Game/Myfiles/Sound/Explosion02.Explosion02"));

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	Streamable.RequestAsyncLoad(
		{ ExplosionEffect.ToSoftObjectPath(), ExplosionSound.ToSoftObjectPath() },
		FStreamableDelegate::CreateUObject(this, &ABoxToSpawn::OnEmitterAndSoundLoaded)
	);
}

void ABoxToSpawn::OnEmitterAndSoundLoaded()
{
	if (ExplosionEffect.IsValid())
	{
		LoadedExplosionEffect = ExplosionEffect.Get();
	}

	if (ExplosionSound.IsValid())
	{
		LoadedExplosionSound = ExplosionSound.Get();
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
	LoadEmitterAndSoundAsync();
}

float ABoxToSpawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth > 0)
	{
		return DamageAmount;
	}

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LoadedExplosionEffect, FTransform(GetActorRotation(),
			GetActorLocation(), FVector(1.f, 1.f, 1.f)),true,EPSCPoolMethod::AutoRelease);
	}
	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, LoadedExplosionSound, GetActorLocation());
	}

	AFPSGameMode* GM = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->AddPlayerScore(ScoreToAward);
		GM->HISMObject->ReturnToPool(this);
	}
	return DamageAmount;

}




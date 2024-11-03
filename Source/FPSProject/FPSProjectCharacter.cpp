// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSProjectCharacter.h"
#include "FPSProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "DebugHelper.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AFPSProjectCharacter::AFPSProjectCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); 
	CameraComponent->bUsePawnControlRotation = true;


	GetMesh()->SetupAttachment(CameraComponent);
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponSocket");
}

void AFPSProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
}



void AFPSProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	MyController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer());
	if (Subsystem && DefaultMappingContext)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{

			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			// Moving
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSProjectCharacter::Move);

			// Looking
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSProjectCharacter::Look);

			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AFPSProjectCharacter::StartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AFPSProjectCharacter::StopFire);
		}
	}
}


void AFPSProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSProjectCharacter::StartFire(const FInputActionValue& Value)
{
	FirstFireDelay = FMath::Max(LastFiredTime + FireRate - GetWorld()->TimeSeconds, 0.f);
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AFPSProjectCharacter::FireEvent, FireRate, !bIsSingleFireWeapon, FirstFireDelay);
}
void AFPSProjectCharacter::FireEvent()
{
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	FVector StartPoint = GetCameraComponent()->GetComponentLocation();
	FVector EndPoint = StartPoint + GetCameraComponent()->GetForwardVector() * TraceDistance;

	LastFiredTime = GetWorld()->TimeSeconds;

	FHitResult OutHit = DoLineTraceByObject(StartPoint, EndPoint);
	FHitResult RealHit = DoLineTraceByObject(MuzzleLocation,
		OutHit.Location == FVector::ZeroVector ? OutHit.TraceEnd : OutHit.Location, true);
	if (OutHit.bBlockingHit && OutHit.GetActor())
	{
		FDamageEvent DamageEvent;
		OutHit.GetActor()->TakeDamage(1.f, DamageEvent, GetController(), this);
	}
}
void AFPSProjectCharacter::StopFire(const FInputActionValue& Value)
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	FireTimerHandle.Invalidate();
}
FHitResult AFPSProjectCharacter::DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug, bool ForDuration, float Duration)
{
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	if (ShowDebug)
	{
		DebugType = EDrawDebugTrace::ForOneFrame;
		if (ForDuration)
		{
			DebugType = EDrawDebugTrace::ForDuration;
		}
	}
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects
	(this, Start, End, TraceObjectTypes, false, TArray<AActor*>(), DebugType, OutHit, true, FColor::Red, FColor::Green, Duration);

	return OutHit;
}
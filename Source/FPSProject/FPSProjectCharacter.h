// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPSProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
struct FHitResult;
class USoundWave;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFPSProjectCharacter : public ACharacter
{
	GENERATED_BODY()
public:
#pragma region Components


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

#pragma endregion

#pragma region Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Core|Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Core|Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
#pragma endregion

#pragma region CoreVariables

	UPROPERTY()
	APlayerController* MyController;

	UPROPERTY(EditDefaultsOnly, Category = "Core|Trace")
	TArray <TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Core|Trace")
	float FireRate = 0.35f;

	UPROPERTY(EditDefaultsOnly, Category = "Core|Trace")
	float TraceDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Core|Trace")
	bool bIsSingleFireWeapon = false;

	UPROPERTY(EditDefaultsOnly, Category = "Core|Weapon")
	USoundWave* FireSound;

	FTimerHandle FireTimerHandle;
	float LastFiredTime;
	float FirstFireDelay;

#pragma endregion
	AFPSProjectCharacter();

protected:
	virtual void BeginPlay();

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void StartFire(const FInputActionValue& Value);
	void FireEvent();
	UFUNCTION()
	void StopFire(const FInputActionValue& Value);
	FHitResult DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug = false, bool ForDuration  = false, float Duration =2.f);

#pragma region Overrides

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

#pragma endregion
public:
#pragma region Getters

	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

#pragma endregion
};


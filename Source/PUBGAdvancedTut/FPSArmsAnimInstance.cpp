// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSArmsAnimInstance.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"


void UFPSArmsAnimInstance::UpdateAnimationProperties(float DeltaTime)
{

    if (ShooterCharacter == nullptr) {
        ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
    }
    if (ShooterCharacter) {
        FVector Velocity { ShooterCharacter->GetVelocity() };
        Velocity.Z = 0;
        Speed = Velocity.Size();

        bAimin=ShooterCharacter->GetIsAiming();
    }
}

void UFPSArmsAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	
}

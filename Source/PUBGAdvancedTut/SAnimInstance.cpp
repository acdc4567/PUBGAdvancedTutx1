// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Rotator.h"


void USAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if (ShooterCharacter == nullptr) {
        ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
    }
    if (ShooterCharacter) {
        FVector Velocity { ShooterCharacter->GetVelocity() };
        Velocity.Z = 0;
        Speed = Velocity.Size();
        Direction = CalculateDirection(Velocity,ShooterCharacter->GetActorRotation());
		bIsPlayingMontage=ShooterCharacter->GetIsPlayingMontage();
		bIsCrouching=ShooterCharacter->GetIsCrouching();
		bIsProne=ShooterCharacter->GetIsProne();
		bIsDead=ShooterCharacter->GetIsDead();
		bIsHoldWeapon=ShooterCharacter->GetIsHoldWeapon();
		bIsAim=ShooterCharacter->GetIsAiming();

		const FRotator DeltaRotator=UKismetMathLibrary::NormalizedDeltaRotator(
			ShooterCharacter->GetControllerxRotation(),ShooterCharacter->GetActorRotation());
		Pitch=DeltaRotator.Pitch;
		Yaw=DeltaRotator.Yaw;

		bIsInAir= ShooterCharacter->GetCharacterMovement()->IsFalling();
		bIsFreeFalling=ShooterCharacter->GetIsFreeFalling();
		bIsUmbrellaOpened=ShooterCharacter->GetIsUmbrellaOpened();
		ForwardValue=ShooterCharacter->GetForwardValue();
		FVector UnrotatedVector=ShooterCharacter->GetActorRotation().UnrotateVector(Velocity);
		SidewaysVelocity=UnrotatedVector.Y;

		//AimOffsetNumber

		if(bIsInAir&&bIsProne){
			AimOffsetType=0;
		}
		else if(bIsCrouching){
			if(bIsHoldWeapon){
				if(bIsAim){
					AimOffsetType=6;
				}else{
					AimOffsetType=4;
				}
			}
			else{
				AimOffsetType=2;
			}
		}
		else{
			if(bIsHoldWeapon){
				if(bIsAim){
					AimOffsetType=5;
				}
				else{
					AimOffsetType=3;
				}
			}
			else{
				AimOffsetType=1;
			}
		}



    }
}

void USAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	
}





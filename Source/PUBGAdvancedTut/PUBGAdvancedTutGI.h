// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PUBGAdvancedTutGI.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API UPUBGAdvancedTutGI : public UGameInstance
{
	GENERATED_BODY()
public:
	UPUBGAdvancedTutGI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Backpack)
	int32 DefaultCapacity=50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunStandName=TEXT("Socket_Gun_Stand");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunAimName=TEXT("Socket_Gun_Aim");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunReloadName=TEXT("Socket_Gun_Reload");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunProneIdleName=TEXT("Socket_Gun_ProneIdle");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunProneFBName=TEXT("Socket_Gun_ProneFB");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunProneOtherName=TEXT("Socket_Gun_ProneOther");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName GunCrouchName=TEXT("Socket_Gun_Crouch");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position)
	FName BackLeftNName=TEXT("Socket_BackLeft_Normal");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position)
	FName BackRightNName=TEXT("Socket_BackRight_Normal");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position)
	FName BackLeftBName=TEXT("Socket_BackLeft_Backpack");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position)
	FName BackRightBName=TEXT("Socket_BackRight_Backpack");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FName HelmetName=TEXT("Socket_Helmet");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FName VestName=TEXT("Socket_Vest");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FName BackpackName=TEXT("Socket_Backpack");

private:




};

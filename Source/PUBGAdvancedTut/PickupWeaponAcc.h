// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupWeaponAcc.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupWeaponAcc : public APickupBase
{
	GENERATED_BODY()



public:
	APickupWeaponAcc();

	virtual int32 GetWeight() override;

	FSTR_ItemWeaponAcc* ItemWeaponAccRow;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

private:
	FString ItemWeaponAccTablePath;

	UDataTable* ItemWeaponAccTableObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	E_WeaponAccType AccType;
	
};

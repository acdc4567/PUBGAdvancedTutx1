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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	E_WeaponAccType AccType;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

private:
	FString ItemWeaponAccTablePath;

	UDataTable* ItemWeaponAccTableObject;

	
	
};

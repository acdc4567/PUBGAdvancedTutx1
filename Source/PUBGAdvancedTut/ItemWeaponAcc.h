// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemWeaponAcc.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemWeaponAcc : public AItemBase
{
	GENERATED_BODY()
public:
	AItemWeaponAcc();

	E_WeaponAccType AccType;

	FSTR_ItemWeaponAcc* ItemWeaponAccRow;

	virtual int32 GetWeight() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;



private:

	FString ItemWeaponAccTablePath;

	UDataTable* ItemWeaponAccTableObject;

	

	

};

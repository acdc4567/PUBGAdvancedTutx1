// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()

public:
	APickupWeapon();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_ItemWeaponx1;

	FString ItemWeaponTablePath;

	UDataTable* ItemWeaponTableObject;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;


public:

	FSTR_ItemWeapon* ItemWeaponRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	int32 Ammo;


};

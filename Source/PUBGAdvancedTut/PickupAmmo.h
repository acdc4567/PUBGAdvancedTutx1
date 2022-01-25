// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupAmmo.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupAmmo : public APickupBase
{
	GENERATED_BODY()
public:
	APickupAmmo();

	FSTR_ItemAmmo* ItemAmmoRow;

	virtual int32 GetWeight() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,meta = (AllowPrivateAccess = "true"))
	int32 SingleWeight;

	FString ItemAmmoTablePath;

	UDataTable* ItemAmmoTableObject;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;


	
};

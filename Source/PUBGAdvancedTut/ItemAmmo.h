// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemAmmo.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemAmmo : public AItemBase
{
	GENERATED_BODY()
	

public:

	AItemAmmo();

	FSTR_ItemAmmo* ItemAmmoRow;

	virtual int32 GetWeight() override;


protected:
	virtual void OnConstruction(const FTransform& Transform) override;


private:
	FString ItemAmmoTablePath;

	UDataTable* ItemAmmoTableObject;


};

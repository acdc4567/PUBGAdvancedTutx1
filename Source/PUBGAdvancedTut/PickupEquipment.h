// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupEquipment.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupEquipment : public APickupBase
{
	GENERATED_BODY()
public:
	APickupEquipment();

	FSTR_ItemEquipment* ItemEquipmentRow;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment,meta = (AllowPrivateAccess = "true"))
	E_ItemType EquipmentType;

	FString ItemEquipmentTablePath;

	UDataTable* ItemEquipmentTableObject;


protected:
	virtual void OnConstruction(const FTransform& Transform) override;
};

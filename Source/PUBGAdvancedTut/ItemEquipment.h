// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemEquipment.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemEquipment : public AItemBase
{
	GENERATED_BODY()
public:
	AItemEquipment();

	FSTR_ItemEquipment* ItemEquipmentRow;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FString ItemEquipmentTablePath;

	UDataTable* ItemEquipmentTableObject;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;


};

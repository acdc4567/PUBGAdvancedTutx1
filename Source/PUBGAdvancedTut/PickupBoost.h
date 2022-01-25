// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupBoost.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupBoost : public APickupBase
{
	GENERATED_BODY()

public:
	APickupBoost();
	FSTR_ItemBoost* ItemBoostRow;

	virtual int32 GetWeight() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health,meta = (AllowPrivateAccess = "true"))
	int32 SingleWeight;

	FString ItemBoostTablePath;

	UDataTable* ItemBoostTableObject;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
};

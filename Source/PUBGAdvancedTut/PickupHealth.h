// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupHealth.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupHealth : public APickupBase
{
	GENERATED_BODY()
public:
	APickupHealth();

	FSTR_ItemHealth* ItemHealthRow;

	virtual int32 GetWeight() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,meta = (AllowPrivateAccess = "true"))
	int32 SingleWeight;

	FString ItemHealthTablePath;

	UDataTable* ItemHealthTableObject;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

};

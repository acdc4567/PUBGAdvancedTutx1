// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemBoost.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemBoost : public AItemBase
{
	GENERATED_BODY()
public:
	AItemBoost();

	FSTR_ItemBoost* ItemBoostRow;

	virtual int32 GetWeight() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FString ItemBoostTablePath;

	UDataTable* ItemBoostTableObject;

	float UseTime;
};

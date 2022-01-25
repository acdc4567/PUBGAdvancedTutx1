// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemHealth.generated.h"

/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemHealth : public AItemBase
{
	GENERATED_BODY()

public:

	AItemHealth();

	FSTR_ItemHealth* ItemHealthRow;

	virtual int32 GetWeight() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FString ItemHealthTablePath;

	UDataTable* ItemHealthTableObject;

	float UseTime;
};

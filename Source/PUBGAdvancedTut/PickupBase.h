// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickupBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOverlapSignature,APickupBase *, PickupObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndOverlapSignature,APickupBase *, PickupObject);


class UStaticMeshComponent;
class UWidgetComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API APickupBase : public AItemBase
{
	GENERATED_BODY()

public:
	APickupBase();

private:
	
	

	UWidgetComponent* UI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Outline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	/** Called when End Overlapping AreaSphere */
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	FText UI_Prefix;

	void InitPickup(E_ItemType ItemType,FText Name,FText UI_Prefix1,UStaticMesh* StaticMeshx1 );

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnBeginOverlapSignature OnBeginOverlap;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnEndOverlapSignature OnEndOverlap;

};

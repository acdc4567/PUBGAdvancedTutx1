// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemWeapon.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class AItemWeaponAcc;
class UAudioComponent;


/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API AItemWeapon : public AItemBase
{
	GENERATED_BODY()


public:

	AItemWeapon();

	UFUNCTION(BlueprintCallable)
	void UpdateMag(AItemWeaponAcc* MagAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateMuzzle(AItemWeaponAcc* MuzzleAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateSight(AItemWeaponAcc* SightAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateForegrip(AItemWeaponAcc* ForegripAccActor);

	UFUNCTION(BlueprintCallable)
	void UpdateButtstock(AItemWeaponAcc* ButtstockAccActor);

	

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayFireSound();

	UFUNCTION(BlueprintCallable)
	void PlayFireFlash();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Muzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Foregrip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Buttstock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FireSound;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* FireFlash;


	FString ItemWeaponTablePath;

	UDataTable* ItemWeaponTableObject;

	AItemWeaponAcc* MagAccActorx1;

	AItemWeaponAcc* MuzzleAccActorx1;

	AItemWeaponAcc* SightAccActorx1;

	AItemWeaponAcc* ForegripAccActorx1;

	AItemWeaponAcc* ButtstockAccActorx1;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	E_WeaponPosition Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon,meta=(ExposeOnSpawn="true"))
	bool bIsOnHand;

	FSTR_ItemWeapon* ItemWeaponRow;

};

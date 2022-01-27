// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponChangedSignature,AItemWeapon *, Weapon, E_WeaponPosition, Position, bool, bIsOnHand );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedSignature, bool, bIsTrue );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Value );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangedSignature, float, Value );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChangedSignature, AItemBase*, Equipment,bool,bIsAdd );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemsInBackpackChangedSignature, AItemBase*, Item,bool,bIsAdd );


class AItemBase;
class AItemWeapon;




/**
 * 
 */
UCLASS()
class PUBGADVANCEDTUT_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:


//Getter Functions
	AItemWeapon* GetWeapon1();

	AItemWeapon* GetWeapon2();

	AItemWeapon* GetHoldGun();

	int32 GetAmmo556();

	int32 GetAmmo762();

	float GetHealthPoint();

	float GetEnergyPoint();

	TArray<AItemBase*> GetEquipments();

	TArray<AItemBase*> GetItemsInBackpack();

//SetterFunctions
	void SetWeapon1(AItemWeapon* Weapon);
	
	UFUNCTION(BlueprintCallable)
	void SetWeapon2(AItemWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void SetHoldGun(AItemWeapon* Weapon);

	void SetAmmo556(int32 Ammo556x);

	void SetAmmo762(int32 Ammo762x);

	void SetHealthPoint(float HP);

	void SetEnergyPoint(float EP);

//ArrayAdd
	UFUNCTION(BlueprintCallable)
	void AddEquipment(AItemBase* Equipment);

	void AddItemsInBackpack(AItemBase* Item);

//ArrayRemove
	UFUNCTION(BlueprintCallable)
	bool RemoveEquipment(AItemBase* Equipment);

	bool RemoveItemsInBackpack(AItemBase* Item);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsWeapon, meta = (AllowPrivateAccess = "true"))
	AItemWeapon* Weapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsWeapon, meta = (AllowPrivateAccess = "true"))
	AItemWeapon* Weapon2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsWeapon, meta = (AllowPrivateAccess = "true"))
	AItemWeapon* HoldGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsAmmo, meta = (AllowPrivateAccess = "true"))
	int32 Ammo556;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsAmmo, meta = (AllowPrivateAccess = "true"))
	int32 Ammo762;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsEquipments, meta = (AllowPrivateAccess = "true"))
	TArray<AItemBase*> Equipments;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemsBackpack, meta = (AllowPrivateAccess = "true"))
	TArray<AItemBase*> ItemsInBackpack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float HealthPoint=100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float EnergyPoint=0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	int32 KilledAmount;


protected:


public:

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnWeaponChangedSignature OnWeaponChanged;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnAmmoChangedSignature OnAmmoChanged;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnEnergyChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnEquipmentChangedSignature OnEquipmentChanged;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnItemsInBackpackChangedSignature OnItemChanged;


};

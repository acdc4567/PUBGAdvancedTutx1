// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWeapon.h"

APickupWeapon::APickupWeapon(){

    ItemWeaponTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemWeapon.DT_ItemWeapon'");
	ItemWeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemWeaponTablePath));	



}

void APickupWeapon::OnConstruction(const FTransform& Transform){
    ItemWeaponRow = nullptr;
    ItemWeaponRow= ItemWeaponTableObject->FindRow<FSTR_ItemWeapon>(ID, TEXT(""));

    FName identifier = TEXT("EQUIP ");
    FText Prefix1 = FText::FromName(identifier);
    
    InitPickup(E_ItemType::EIT_Weapon,ItemWeaponRow->Name,Prefix1,ItemWeaponRow->StaticMesh);
}
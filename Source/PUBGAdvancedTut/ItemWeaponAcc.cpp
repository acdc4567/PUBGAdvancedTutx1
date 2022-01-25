// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeaponAcc.h"

AItemWeaponAcc::AItemWeaponAcc(){
    ItemWeaponAccTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemWeaponAcc.DT_ItemWeaponAcc'");
	ItemWeaponAccTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemWeaponAccTablePath));	

}

int32 AItemWeaponAcc::GetWeight(){
    ItemWeaponAccRow= ItemWeaponAccTableObject->FindRow<FSTR_ItemWeaponAcc>(ID, TEXT(""));
    return ItemWeaponAccRow->Weight;
}

void AItemWeaponAcc::OnConstruction(const FTransform& Transform){
    ItemWeaponAccRow = nullptr;
    ItemWeaponAccRow= ItemWeaponAccTableObject->FindRow<FSTR_ItemWeaponAcc>(ID, TEXT(""));

    
    AccType=ItemWeaponAccRow->Type;
    
    Init(E_ItemType::EIT_Accessories,ItemWeaponAccRow->Name);

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemAmmo.h"

AItemAmmo::AItemAmmo()
{

    ItemAmmoTablePath = TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemAmmo.DT_ItemAmmo'");
    ItemAmmoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemAmmoTablePath));
}


int32 AItemAmmo::GetWeight(){
    
    
    return ItemAmmoRow->Weight*Amount;
}

void AItemAmmo::OnConstruction(const FTransform& Transform){
    ItemAmmoRow = nullptr;
    ItemAmmoRow= ItemAmmoTableObject->FindRow<FSTR_ItemAmmo>(ID, TEXT(""));
   
    
    Init(E_ItemType::EIT_Ammo,ItemAmmoRow->Name);
}






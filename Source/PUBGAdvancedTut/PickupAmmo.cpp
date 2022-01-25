// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAmmo.h"

APickupAmmo::APickupAmmo(){

    ItemAmmoTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemAmmo.DT_ItemAmmo'");
	ItemAmmoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemAmmoTablePath));	

}

int32 APickupAmmo::GetWeight(){
    
    
    return SingleWeight*Amount;
}

void APickupAmmo::OnConstruction(const FTransform& Transform){
    ItemAmmoRow = nullptr;
    ItemAmmoRow= ItemAmmoTableObject->FindRow<FSTR_ItemAmmo>(ID, TEXT(""));
    if (Amount == 0) {
        Amount = ItemAmmoRow->PickupAmmo;
    }

    SingleWeight=ItemAmmoRow->Weight;

    FName identifier = TEXT("PICK UP ");
    FText Prefix1 = FText::FromName(identifier);
    
    InitPickup(E_ItemType::EIT_Ammo,ItemAmmoRow->Name,Prefix1,ItemAmmoRow->PickupMesh);
}
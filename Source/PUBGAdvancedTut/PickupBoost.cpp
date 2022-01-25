// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBoost.h"

APickupBoost::APickupBoost(){
    ItemBoostTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemBoost.DT_ItemBoost'");
	ItemBoostTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemBoostTablePath));	

}

int32 APickupBoost::GetWeight(){
    
    
    return SingleWeight*Amount;
}

void APickupBoost::OnConstruction(const FTransform& Transform){
    ItemBoostRow = nullptr;
    ItemBoostRow= ItemBoostTableObject->FindRow<FSTR_ItemBoost>(ID, TEXT(""));

    SingleWeight=ItemBoostRow->Weight;
   
    FName identifier = TEXT("PICK UP ");
    FText Prefix1 = FText::FromName(identifier);
    
    InitPickup(E_ItemType::EIT_Boost,ItemBoostRow->Name,Prefix1,ItemBoostRow->StaticMesh);


}
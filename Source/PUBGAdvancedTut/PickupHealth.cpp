// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupHealth.h"

APickupHealth::APickupHealth(){
    ItemHealthTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemHealth.DT_ItemHealth'");
	ItemHealthTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemHealthTablePath));	

}

int32 APickupHealth::GetWeight(){
    
    
    return SingleWeight*Amount;
}

void APickupHealth::OnConstruction(const FTransform& Transform){
    ItemHealthRow = nullptr;
    ItemHealthRow= ItemHealthTableObject->FindRow<FSTR_ItemHealth>(ID, TEXT(""));

    SingleWeight=ItemHealthRow->Weight;
   
    FName identifier = TEXT("PICK UP ");
    FText Prefix1 = FText::FromName(identifier);
    
    InitPickup(E_ItemType::EIT_Health,ItemHealthRow->Name,Prefix1,ItemHealthRow->StaticMesh);


}
 
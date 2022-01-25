// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHealth.h"

AItemHealth::AItemHealth(){
    ItemHealthTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemHealth.DT_ItemHealth'");
	ItemHealthTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemHealthTablePath));	

}


int32 AItemHealth::GetWeight(){
return ItemHealthRow->Weight*Amount;
}

void AItemHealth::OnConstruction(const FTransform& Transform){
    ItemHealthRow = nullptr;
    ItemHealthRow= ItemHealthTableObject->FindRow<FSTR_ItemHealth>(ID, TEXT(""));

    
    
    Init(E_ItemType::EIT_Health,ItemHealthRow->Name);
    UseTime=ItemHealthRow->UseTime;

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBoost.h"

AItemBoost::AItemBoost(){
    ItemBoostTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemBoost.DT_ItemBoost'");
	ItemBoostTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemBoostTablePath));	

}


int32 AItemBoost::GetWeight(){
    return ItemBoostRow->Weight*Amount;
}

void AItemBoost::OnConstruction(const FTransform& Transform){
    ItemBoostRow = nullptr;
    ItemBoostRow= ItemBoostTableObject->FindRow<FSTR_ItemBoost>(ID, TEXT(""));

    
    
    Init(E_ItemType::EIT_Boost,ItemBoostRow->Name);
    UseTime=ItemBoostRow->UseTime;

}
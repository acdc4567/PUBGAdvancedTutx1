// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEquipment.h"
#include "Components/StaticMeshComponent.h"

AItemEquipment::AItemEquipment(){
    ItemEquipmentTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemEquipment.DT_ItemEquipment'");
	ItemEquipmentTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemEquipmentTablePath));	

    StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AItemEquipment::OnConstruction(const FTransform& Transform){

    ItemEquipmentRow = nullptr;
    ItemEquipmentRow= ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(ID, TEXT(""));
    
    Init(ItemEquipmentRow->Type,ItemEquipmentRow->Name);
    StaticMesh->SetStaticMesh(ItemEquipmentRow->PickupMesh);
}
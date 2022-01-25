// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "ItemBase.h"
#include "ItemWeapon.h"

AItemWeapon* ASPlayerState:: GetWeapon1(){
    return Weapon1;
}
AItemWeapon* ASPlayerState:: GetWeapon2(){
    return Weapon2;
}

AItemWeapon* ASPlayerState:: GetHoldGun(){
    return HoldGun;
}

int32 ASPlayerState::GetAmmo556(){
    return Ammo556;
}

int32 ASPlayerState::GetAmmo762(){
    return Ammo762;
}



float ASPlayerState::GetHealthPoint(){
    return HealthPoint;
}

float ASPlayerState::GetEnergyPoint(){
    return EnergyPoint;
}

TArray<AItemBase*> ASPlayerState::GetEquipments(){
    return Equipments;
}


TArray<AItemBase*> ASPlayerState::GetItemsInBackpack(){
    return ItemsInBackpack;
}

//Setters
void ASPlayerState::SetWeapon1(AItemWeapon* Weapon){
    Weapon1=Weapon;
    OnWeaponChanged.Broadcast(Weapon1,E_WeaponPosition::EWP_Left,false);
}

void ASPlayerState::SetWeapon2(AItemWeapon* Weapon){
    Weapon2=Weapon;
    OnWeaponChanged.Broadcast(Weapon2,E_WeaponPosition::EWP_Right,false);

}

void ASPlayerState::SetHoldGun(AItemWeapon* Weapon){
    HoldGun=Weapon;
    if(HoldGun){
        OnWeaponChanged.Broadcast(HoldGun,HoldGun->Position,true);
    }
    else{
        OnWeaponChanged.Broadcast(HoldGun,E_WeaponPosition::EWP_Left,true);
    }
}

void ASPlayerState::SetAmmo556(int32 Ammo556x){
    Ammo556 = Ammo556x;
    OnAmmoChanged.Broadcast(true);
}

void ASPlayerState::SetAmmo762(int32 Ammo762x){
    Ammo762 = Ammo762x;
    OnAmmoChanged.Broadcast(true);
}


void ASPlayerState::SetHealthPoint(float HP){
    HealthPoint=HP;
    OnHealthChanged.Broadcast(HealthPoint);
}

void ASPlayerState::SetEnergyPoint(float EP){
    EnergyPoint=EP;
    OnEnergyChanged.Broadcast(EnergyPoint);
}
//ArrayAdd

void ASPlayerState::AddEquipment(AItemBase* Equipment){
    Equipments.Add(Equipment);
    OnEquipmentChanged.Broadcast(Equipment,true);
}

void ASPlayerState::AddItemsInBackpack(AItemBase* Item){
    ItemsInBackpack.Add(Item);
    OnItemChanged.Broadcast(Item,true);
}

//ArrayRemove

bool ASPlayerState::RemoveEquipment(AItemBase* Equipment){
    Equipments.Remove(Equipment);
    OnEquipmentChanged.Broadcast(Equipment,false);
    return true;
}

bool ASPlayerState::RemoveItemsInBackpack(AItemBase* Item){
    ItemsInBackpack.Remove(Item);
    OnItemChanged.Broadcast(Item,false);
    return true;
}


//ASPlayerState

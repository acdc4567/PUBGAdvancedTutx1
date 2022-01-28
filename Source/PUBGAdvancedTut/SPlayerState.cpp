// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "ItemBase.h"
#include "ItemWeapon.h"
#include "Kismet/GameplayStatics.h"

#include "PUBGAdvancedTutGI.h"

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
    UpdateAmmoObject();
    OnAmmoChanged.Broadcast(true);
}

void ASPlayerState::SetAmmo762(int32 Ammo762x){
    Ammo762 = Ammo762x;
    UpdateAmmoObject();
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



bool ASPlayerState::CheckBackpackCapacity(int32 AddWeight){
    GameInstanceRef=Cast<UPUBGAdvancedTutGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	ItemEquipmentTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemEquipment.DT_ItemEquipment'");
	ItemEquipmentTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemEquipmentTablePath));	

    int32 TotalCapacity=GameInstanceRef->DefaultCapacity;
    int32 ItemsWeight=0;
    for(int32 i=0;i<Equipments.Num();i++){
        if(Equipments[i]->ItemType==E_ItemType::EIT_Backpack){
            ItemEquipmentRow=ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(Equipments[i]->ID, TEXT(""));
            TotalCapacity+=ItemEquipmentRow->Capacity;
        }
    }

    for(int32 i=0;i<ItemsInBackpack.Num();i++){
        ItemsWeight+=ItemsInBackpack[i]->GetWeight();
    }
    if(AddWeight+ItemsWeight<=TotalCapacity){
        return true;
    }
    else{
        return false;
    }
    return false;
}







void ASPlayerState::UpdateAmmoObject(){
    AItemBase* Ammo1;
    AItemBase* Ammo2;
    for(int32 i=0;i<ItemsInBackpack.Num();i++){
        if(ItemsInBackpack[i]->ItemType==E_ItemType::EIT_Ammo){
            if(ItemsInBackpack[i]->ID==TEXT("1")){
                Ammo1=ItemsInBackpack[i];
            }
            else{
                Ammo2=ItemsInBackpack[i];
            }
        }    
    }
    //Ammo1
    if(GetAmmo556()>0){
        if(Ammo1){
            Ammo1->Amount=GetAmmo556();
        }
        else{
            AItemAmmo* TempAmmo;
            FTransform TempTransform;
            TempTransform.SetLocation(FVector::ZeroVector);
            TempTransform.SetRotation(FQuat(0,0,0,0));
            TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
            TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
            if(TempAmmo){
                
                
                TempAmmo->ID=TEXT("1");
                
                TempAmmo->SN=GameInstanceRef->GenrateSN();
                TempAmmo->Amount=GetAmmo556();	
                
                
                UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
            }
            AItemBase* TempItemBase=Cast<AItemBase>(TempAmmo);
            AddItemsInBackpack(TempItemBase);
        }
        
    }
    else{
        if(Ammo1){
            
            RemoveItemsInBackpack(Ammo1);
            bool bIsDestroyed=Cast<AActor>(Ammo1)->Destroy();
		
        }
    }

    //Ammo2
    if(GetAmmo762()>0){
        if(Ammo2){
            Ammo2->Amount=GetAmmo762();
        }
        else{
            AItemAmmo* TempAmmo;
            FTransform TempTransform;
            TempTransform.SetLocation(FVector::ZeroVector);
            TempTransform.SetRotation(FQuat(0,0,0,0));
            TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
            TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
            if(TempAmmo){
                
                
                TempAmmo->ID=TEXT("2");
                
                TempAmmo->SN=GameInstanceRef->GenrateSN();
                TempAmmo->Amount=GetAmmo762();	
                
                
                UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
            }
            AItemBase* TempItemBase=Cast<AItemBase>(TempAmmo);
            AddItemsInBackpack(TempItemBase);
        }
        
    }
    else{
        if(Ammo2){
            
            RemoveItemsInBackpack(Ammo2);
            bool bIsDestroyed=Cast<AActor>(Ammo2)->Destroy();
		
        }
    }



}

void ASPlayerState::UpdateAmmoAmount(FName IDx1,bool bIsAdd,int32 Amountx1){
    if(Amountx1==0) {
        if(IDx1==TEXT("1")){
            SetAmmo556(0);
        }
        else if(IDx1==TEXT("2")){
            SetAmmo762(0);
        }
    }
    else if(bIsAdd){
        if(IDx1==TEXT("1")){
            Amountx1+=GetAmmo556();
            SetAmmo556(Amountx1);
        }
        else if(IDx1==TEXT("2")){
            Amountx1+=GetAmmo762();
            SetAmmo762(Amountx1);
        }
    }
    else if(!bIsAdd){
        if(IDx1==TEXT("1")){
            Amountx1-=GetAmmo556();
            SetAmmo556(Amountx1);
        }
        else if(IDx1==TEXT("2")){
            Amountx1-=GetAmmo762();
            SetAmmo762(Amountx1);
        }
    }
}




bool ASPlayerState::CheckReplaceBackpack(AItemBase* Item){
    int32 ItemsWeight=0;
    int32 PickupCapacity=0;
    TArray<AItemBase*> ItemsInBag= GetItemsInBackpack();
    for(int32 i=0;i<ItemsInBag.Num();i++){
        ItemsWeight+= ItemsInBag[i]->GetWeight();
    }
    if(Item){
        ItemEquipmentTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemEquipment.DT_ItemEquipment'");
        ItemEquipmentTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemEquipmentTablePath));	
        ItemEquipmentRow=ItemEquipmentTableObject->FindRow<FSTR_ItemEquipment>(Item->ID, TEXT(""));
        PickupCapacity= ItemEquipmentRow->Capacity;
    }
    else{
        PickupCapacity=0;
    }
    if(PickupCapacity+GameInstanceRef->DefaultCapacity>=ItemsWeight){
        return true;
    }
    return false;

}








//ASPlayerState

// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


AItemWeapon::AItemWeapon(){

    SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
    SkeletalMesh->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
    
    Mag=CreateDefaultSubobject<UStaticMeshComponent>("Mag");
    Mag->SetupAttachment(SkeletalMesh);
    Mag->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Muzzle=CreateDefaultSubobject<UStaticMeshComponent>("Muzzle");
    Muzzle->SetupAttachment(SkeletalMesh);
    Muzzle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Sight=CreateDefaultSubobject<UStaticMeshComponent>("Sight");
    Sight->SetupAttachment(SkeletalMesh);
    Sight->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Foregrip=CreateDefaultSubobject<UStaticMeshComponent>("Foregrip");
    Foregrip->SetupAttachment(SkeletalMesh);
    Foregrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Buttstock=CreateDefaultSubobject<UStaticMeshComponent>("Buttstock");
    Buttstock->SetupAttachment(SkeletalMesh);
    Buttstock->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ItemWeaponTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ItemWeapon.DT_ItemWeapon'");
	ItemWeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemWeaponTablePath));	

    FireSound=CreateDefaultSubobject<UAudioComponent>("FireSound");

    
}

void AItemWeapon::OnConstruction(const FTransform& Transform){
    ItemWeaponRow = nullptr;
    ItemWeaponRow= ItemWeaponTableObject->FindRow<FSTR_ItemWeapon>(ID, TEXT(""));

    
    
    Init(E_ItemType::EIT_Weapon,ItemWeaponRow->Name);
    SkeletalMesh->SetSkeletalMesh(ItemWeaponRow->SkeletalMesh);
    Mag->SetStaticMesh(ItemWeaponRow->DefaultMag);
    Mag->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Mag"));
    Muzzle->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Muzzle"));

    if(ItemWeaponRow->DefaultSight){
        Sight->SetStaticMesh(ItemWeaponRow->DefaultSight);
        Sight->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Sight_Default"));

    }
    if(ItemWeaponRow->bCanGrip){
        Foregrip->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Foregrip"));

    }
    if(ItemWeaponRow->DefaultButtstock){
        Buttstock->SetStaticMesh(ItemWeaponRow->DefaultButtstock);
        Buttstock->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Buttstock"));
    }

    FireInterval=ItemWeaponRow->FiringInterval;
    
}

void AItemWeapon::UpdateMag(AItemWeaponAcc* MagAccActor){
    MagAccActorx1=MagAccActor;
    if(MagAccActorx1==nullptr){
        Mag->SetStaticMesh(ItemWeaponRow->DefaultMag);
    }
    else if(ID!=TEXT("2")){
        Mag->SetStaticMesh( MagAccActorx1->ItemWeaponAccRow->StaticMesh);
        Mag->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),MagAccActor->ItemWeaponAccRow->SocketName);

    }
    else{
        Mag->SetStaticMesh( MagAccActorx1->ItemWeaponAccRow->StaticMesh2);
        Mag->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),MagAccActor->ItemWeaponAccRow->SocketName);


    }
}

void AItemWeapon::UpdateMuzzle(AItemWeaponAcc* MuzzleAccActor){
    MuzzleAccActorx1=MuzzleAccActor;

    if(MuzzleAccActorx1==nullptr){
        
    }
    else {
        Muzzle->SetStaticMesh(MuzzleAccActorx1->ItemWeaponAccRow->StaticMesh);
        Muzzle->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),MuzzleAccActor->ItemWeaponAccRow->SocketName);

    }
}

void AItemWeapon::UpdateSight(AItemWeaponAcc* SightAccActor){
    SightAccActorx1=SightAccActor;
    if(SightAccActorx1==nullptr){
        Sight->SetStaticMesh(ItemWeaponRow->DefaultSight);
        Sight->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Sight_Default"));

    }
    else {
        Sight->SetStaticMesh(SightAccActor->ItemWeaponAccRow->StaticMesh);
        Sight->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),SightAccActor->ItemWeaponAccRow->SocketName);

    }
}

void AItemWeapon::UpdateForegrip(AItemWeaponAcc* ForegripAccActor){
    ForegripAccActorx1=ForegripAccActor;
    if(ForegripAccActorx1==nullptr){

    }
    else{
        Foregrip->SetStaticMesh(ForegripAccActor->ItemWeaponAccRow->StaticMesh);
        Foregrip->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),ForegripAccActor->ItemWeaponAccRow->SocketName);

    }
}

void AItemWeapon::UpdateButtstock(AItemWeaponAcc* ButtstockAccActor){
    ButtstockAccActorx1=ButtstockAccActor;
    if(ButtstockAccActorx1==nullptr){
        Buttstock->SetStaticMesh(ItemWeaponRow->DefaultButtstock);
        Buttstock->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),TEXT("Socket_Buttstock"));

    }
    else{
        Buttstock->SetStaticMesh(ButtstockAccActor->ItemWeaponAccRow->StaticMesh);
        Buttstock->AttachToComponent(SkeletalMesh,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),ButtstockAccActor->ItemWeaponAccRow->SocketName);

    }
}

void AItemWeapon::PlayFireSound(){
    if(MuzzleAccActorx1!=nullptr){
        if (MuzzleAccActorx1->ItemWeaponAccRow->bIsSuppressor) {
            FireSound->SetSound(ItemWeaponRow->FireSoundSuppressor);

        } 
        else {
            FireSound->SetSound(ItemWeaponRow->FireSound);
        }
    } 
    else {
        FireSound->SetSound(ItemWeaponRow->FireSound);
    }
    FireSound->Play();
}

void AItemWeapon::PlayFireFlash(){
    if(MuzzleAccActorx1!=nullptr){
        if(!MuzzleAccActorx1->ItemWeaponAccRow->bIsNotFlash){
            FireFlash=ItemWeaponRow->FireFlash;
            if (FireFlash)
			{
                const FTransform SocketTransform = SkeletalMesh->GetSocketTransform(TEXT("Socket_Muzzle"));
                UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), FireFlash, SocketTransform.GetLocation());
            }
        }
    } 
    else {
        FireFlash = ItemWeaponRow->FireFlash;
        if (FireFlash) {
            const FTransform SocketTransform = SkeletalMesh->GetSocketTransform(TEXT("Socket_Muzzle"));
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireFlash, SocketTransform.GetLocation());
        }
    }
}


void AItemWeapon::SwitchShootMode(){
    if(ItemWeaponRow->bAutoMode){
        if(ShootModex1==E_ShootMode::ESM_Single){
            ShootModex1=E_ShootMode::ESM_Auto;
        }
        else{
            ShootModex1=E_ShootMode::ESM_Single;
        }
    }
}







void AItemWeapon::PressFire(){
    FireGate.Open();
    AutoFire();


}

void AItemWeapon::AutoFire(){

    if(FireGate.IsGateOpen()){
        if(ShootModex1==E_ShootMode::ESM_Auto){
            GetWorldTimerManager().SetTimer(TH_FireTimerInProgress, this, &AItemWeapon::AutoFire, FireInterval);
        }
        if(Ammo>=0){
            PlayFireSound();
            PlayFireFlash();
            //--Ammo;
        }
        else{
            UE_LOG(LogTemp,Warning,TEXT("NoBullets"));
        }
    }

}

void AItemWeapon::ReleaseFire(){
    FireGate.Close();


}





//AItemWeapon

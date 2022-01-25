// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent=SceneComponent;
	
	

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterRef=Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
	
}

void AItemBase::Init(E_ItemType ItemTypex1,FText Namex1){

	ItemType=ItemTypex1;
	Name=Namex1;


}


int32 AItemBase::GetWeight(){
	return 0;
}


void AItemBase::UpdateAmount(bool bIsAdd,bool bIsReduce,int32 Amountx1){
    if (bIsAdd) {
        Amount += Amountx1;
    }
	else if(bIsReduce){
		Amount-=Amountx1;
	}
	else{
		Amount=Amountx1;
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





//AItemBase

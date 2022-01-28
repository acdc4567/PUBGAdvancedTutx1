// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "ItemWeapon.h"
#include "ItemHealth.h"
#include "ItemBoost.h"
#include "ItemEquipment.h"
#include "PickupBase.h"
#include "PickupWeapon.h"
#include "PickupWeaponAcc.h"
#include "PickupAmmo.h"
#include "PickupBoost.h"
#include "PickupEquipment.h"
#include "PickupHealth.h"
#include "SPlayerState.h"
#include "PUBGAdvancedTutGI.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; 
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	GetCharacterMovement()->bUseSeparateBrakingFriction=true;

	ProneTimeTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_ProneTime.DT_ProneTime'");
	ProneTimeTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ProneTimeTablePath));	

	WalkSpeedTablePath=TEXT("DataTable'/Game/Blueprint/Datas/DataTable/DT_WalkSpeed.DT_WalkSpeed'");
	WalkSpeedTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WalkSpeedTablePath));	

	
	
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(Controller!=nullptr){
		PlayerStateRef=Cast<ASPlayerState>(Controller->PlayerState);
	}
	GameInstanceRef=Cast<UPUBGAdvancedTutGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	
	
}

void ASCharacter::MoveForward(float AxisValue){
	
    if ((Controller != nullptr) && (AxisValue != 0.0f)) {
        FRotator Rotation { Controller->GetControlRotation() };
        if (bAltKeyPressed) {
            Rotation = AltKeyPressedRotation;
        } else {
            Rotation = Controller->GetControlRotation();
        }

        const FRotator YawRotation { 0, Rotation.Yaw, 0 };
        SmoothIncrease();
        const FVector Direction { FRotationMatrix { YawRotation }.GetUnitAxis(EAxis::X) };
        
		if(bEnableMove){
			HandleWalkSpeedFromTable();
			AddMovementInput(Direction, AxisValue);
		}	
		
    
	}
	if(MoveForwardAxis!=AxisValue){
		MoveForwardAxis=AxisValue;
		UpdateCameraHeight();
		FName TempName=CalculateHoldGunSocket();
		UpdateWeaponDisplay(TempName); 
	}

}




void ASCharacter::MoveRight(float AxisValue){
	
    if ((Controller != nullptr) && (AxisValue != 0.0f)) {

        // find out which way is forward
        FRotator Rotation { Controller->GetControlRotation() };

        if (bAltKeyPressed) {
            Rotation = AltKeyPressedRotation;
        } else {
            Rotation = Controller->GetControlRotation();
        }
        const FRotator YawRotation { 0, Rotation.Yaw, 0 };
        SmoothIncrease();
        const FVector Direction { FRotationMatrix { YawRotation }.GetUnitAxis(EAxis::Y) };

        if (bEnableMove) {
			HandleWalkSpeedFromTable();
            AddMovementInput(Direction, AxisValue);
        }

    }
	if(MoveRightAxis!=AxisValue){
		MoveRightAxis=AxisValue;
		UpdateCameraHeight();
		FName TempName=CalculateHoldGunSocket();
		UpdateWeaponDisplay(TempName);
	}

}

void ASCharacter::SmoothIncrease(){

    FVector Velocity { GetCharacterMovement()->Velocity };
    Velocity.Z = 0;
    float Speed = Velocity.Size();
    Speed += 10.f;
    if (Speed <= WalkSpeed) {
        GetCharacterMovement()->MaxWalkSpeed = Speed;
    }
}

void ASCharacter::AltKeyPressed(){
	bAltKeyPressed = true;
	
	AltKeyPressedRotation=CameraBoom->GetTargetRotation();
	
	
}

void ASCharacter::AltKeyReleased(){
	bAltKeyPressed = false;
	
	AltKeyReleasedRotation=CameraBoom->GetTargetRotation();
	HandleAltKeyReleased(AltKeyPressedRotation,AltKeyReleasedRotation);
	
}

void ASCharacter::CrouchKeyPressed(){
	
	if(bIsProne){
		bIsProne=false;
		bIsCrouching=true;
		HandleProneTimeFromTable(3,2);
	}
	else if(bIsCrouching){
		bIsCrouching=false;
	}
	else{
		bIsCrouching=true;
		
	}
	LimitPitchAngle(0.f);
	UpdateCameraHeight();
	FName TempName=CalculateHoldGunSocket();
	UpdateWeaponDisplay(TempName);
}

void ASCharacter::WalkKeyPressed() { 
	bWalkPressed = true;
	SmoothIncrease();
}

void ASCharacter::WalkKeyReleased() {
	 bWalkPressed = false;
	 SmoothIncrease();
}

void ASCharacter::RunKeyPressed() { 
	bRunPressed = true;
	SmoothIncrease();
}

void ASCharacter::RunKeyReleased() { 
	bRunPressed = false;
	SmoothIncrease();
}

void ASCharacter::ProneKeyPressed(){
	
	if(bIsProne){
		bIsProne=false;
		HandleProneTimeFromTable(3,1);
	}
	else if(bIsCrouching){
		bIsCrouching=false;
		bIsProne=true;
		bIsAiming=false;
		HandleProneTimeFromTable(2,3);
	}
	else{
		bIsProne=true;
		bIsAiming=false;
		HandleProneTimeFromTable(1,3);
	}
	LimitPitchAngle(0.f);
	UpdateCameraHeight();
	FName TempName=CalculateHoldGunSocket();
	UpdateWeaponDisplay(TempName);

}

FRotator ASCharacter::GetControllerxRotation(){
	if(Controller!=nullptr){
		return Controller->GetControlRotation();
	}
	return FRotator::ZeroRotator;
}

void ASCharacter::JumpKeyPressed(){
    if (bIsProne) { 
		bIsProne=false;
		bIsCrouching=true;
		HandleProneTimeFromTable(3,2);
	}
	else if(bIsCrouching){
		bIsCrouching=false;
	}
	else{
		Jump();
	}
	UpdateCameraHeight();

}

void ASCharacter::AimKeyPressed(){
	bIsAiming=true;
	FName TempName=CalculateHoldGunSocket();
	UpdateWeaponDisplay(TempName);
}

void ASCharacter::AimKeyReleased(){
	bIsAiming=false;
	FName TempName=CalculateHoldGunSocket();
	UpdateWeaponDisplay(TempName);
}

void ASCharacter::EnableMove(){
	bEnableMove=true;
	GetWorldTimerManager().ClearTimer(TH_DisableMove);

}

void ASCharacter::Turn(float Value)
{
    float TurnScaleFactor {};
    if (bIsAiming) {
        TurnScaleFactor = MouseAimTurnRate;
    } else {
        TurnScaleFactor = MouseHipTurnRate;
    }
	
	AddControllerYawInput(Value * TurnScaleFactor);
	
    
}

void ASCharacter::LookUp(float Value)
{
    float LookUpScaleFactor {};
    if (bIsAiming) {
        LookUpScaleFactor = MouseAimLookUpRate;
    } else {
        LookUpScaleFactor = MouseHipLookUpRate;
    }
	if(LimitPitchAngle(Value)){
		AddControllerPitchInput(Value * LookUpScaleFactor);
	}
    
}






void ASCharacter::UpdateCameraHeight(){

	if(bIsCrouching){
		if(!bIsHoldWeapon&&(MoveForwardAxis!=0||MoveRightAxis!=0)){
			NewCameraHeight=CrouchRunCameraHeight;
		}
		else if(bIsHoldWeapon&&(MoveForwardAxis!=0||MoveRightAxis!=0)){
			NewCameraHeight=CrouchRifleMoveCameraHeight;
		}
		else{
			NewCameraHeight=CrouchCameraHeight;
		}
	}
	else if(bIsProne){
		NewCameraHeight=ProneCameraHeight;
	}
	else{
		NewCameraHeight=StandCameraHeight;
	}


	//NextSequence
	if(CurrentCameraHeight!=NewCameraHeight){
		OriginalCameraHeight= CurrentCameraHeight;
		HandleCameraHeightChanged(OriginalCameraHeight,NewCameraHeight);
	}



}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TargetingItem();
	

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::LookUp);

	PlayerInputComponent->BindAction("Alt",IE_Pressed,this,&ASCharacter::AltKeyPressed);
	PlayerInputComponent->BindAction("Alt",IE_Released,this,&ASCharacter::AltKeyReleased);

	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ASCharacter::CrouchKeyPressed);

	PlayerInputComponent->BindAction("Prone",IE_Pressed,this,&ASCharacter::ProneKeyPressed);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::JumpKeyPressed);

	PlayerInputComponent->BindAction("Walk",IE_Pressed,this,&ASCharacter::WalkKeyPressed);
	PlayerInputComponent->BindAction("Walk",IE_Released,this,&ASCharacter::WalkKeyReleased);

	PlayerInputComponent->BindAction("LeftShift",IE_Pressed,this,&ASCharacter::RunKeyPressed);
	PlayerInputComponent->BindAction("LeftShift",IE_Released,this,&ASCharacter::RunKeyReleased);

	PlayerInputComponent->BindAction("Aiming",IE_Pressed,this,&ASCharacter::AimKeyPressed);
	PlayerInputComponent->BindAction("Aiming",IE_Released,this,&ASCharacter::AimKeyReleased);

	PlayerInputComponent->BindAction("DiscardWeapon",IE_Pressed,this,&ASCharacter::BeginDiscard);
	
	PlayerInputComponent->BindAction("Interaction",IE_Pressed,this,&ASCharacter::BeginPickupItem);
	
	PlayerInputComponent->BindAction("TakeBackGun",IE_Pressed,this,&ASCharacter::BeginPlayMontage);
	
	PlayerInputComponent->BindAction("Keyboard1",IE_Pressed,this,&ASCharacter::Keyboard1KeyPressed);
	
	PlayerInputComponent->BindAction("Keyboard2",IE_Pressed,this,&ASCharacter::Keyboard2KeyPressed);
	



}

bool ASCharacter::LimitPitchAngle(float Axis){
	float BottomValue;
	float TopValue;
	if(bIsProne){
		BottomValue=ProneBottomValue;
		TopValue=ProneTopValue;

	}
	else if(bIsCrouching ){
		BottomValue=CrouchBottomValue;
		TopValue=CrouchTopValue;
	}
	else{
		BottomValue=StandBottomValue;
		TopValue=StandTopValue;
	}

	if(Controller!=nullptr){
		float ControllerPitch=Controller->GetControlRotation().Pitch;
		APlayerController* PC=Cast<APlayerController>(Controller);
		float Excess=Axis*PC->InputPitchScale;
		if(!(ControllerPitch>BottomValue||ControllerPitch<TopValue)){
			if(FMath::Abs(TopValue-ControllerPitch)<FMath::Abs(BottomValue-ControllerPitch)){
				FRotator NewRotator=FRotator(Controller->GetControlRotation().Roll,TopValue,Controller->GetControlRotation().Yaw);
				Controller->SetControlRotation(NewRotator);
			}
			else
			{
				FRotator NewRotator=FRotator(Controller->GetControlRotation().Roll,BottomValue,Controller->GetControlRotation().Yaw);
				Controller->SetControlRotation(NewRotator);	
			}
			
		}
		
		if(ControllerPitch+Excess>BottomValue||ControllerPitch+Excess<TopValue){
			return true;
		}else{
			return false;
		}
	}

	return true;
}

void ASCharacter::HandleProneTimeFromTable(int32 Pose1,int32 Pose2){

//PoseNumber1=Stand,PoseNumber2=Crouch,PoseNumber3=Prone

	FSTR_ProneTime* ProneTimeRow = nullptr;
	if(Pose1==3&&Pose2==2){
		if(bIsHoldWeapon){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_prone_crouch"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_prone_crouch"), TEXT(""));

		}
	}
	if(Pose1==3&&Pose2==1){
		if(bIsHoldWeapon){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_prone_stand"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_prone_stand"), TEXT(""));

		}
	}
	if(Pose1==2&&Pose2==3){
		if(bIsHoldWeapon){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_crouch_prone"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_crouch_prone"), TEXT(""));

		}
	}
	if(Pose1==1&&Pose2==3){
		if(bIsHoldWeapon){
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("true_stand_prone"), TEXT(""));

		}else{
			ProneTimeRow = ProneTimeTableObject->FindRow<FSTR_ProneTime>(FName("false_stand_prone"), TEXT(""));

		}
	}
	if(ProneTimeRow){
		DisableMoveTime=ProneTimeRow->Sec;
		
	}


	bEnableMove=false;
	GetWorldTimerManager().SetTimer(TH_DisableMove,this,&ASCharacter::EnableMove,DisableMoveTime,false);


}

void ASCharacter::HandleWalkSpeedFromTable(){
//PoseNumber1=Stand,PoseNumber2=Crouch,PoseNumber3=Prone
//WalkRun1=Walk,WalkRun2=Jog,WalkRun3=Run,WalkRun4=Aim

	FSTR_WalkSpeed* WalkSpeedRow = nullptr;
	if(bIsHoldWeapon){
		if(bIsCrouching){
			if(bIsAiming){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_aim"), TEXT(""));

			}
			else if(bWalkPressed){
				
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_crouch_jog"), TEXT(""));

			}
			
		}
		else if(bIsProne){
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_prone_jog"), TEXT(""));

			}

		}
		else{
			if(bIsAiming){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_aim"), TEXT(""));

			}
			else if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_run"), TEXT(""));
				
			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("true_stand_jog"), TEXT(""));

			}
		}
	}
	else{
		if(bIsCrouching){
			
			if(bWalkPressed){
				
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_crouch_jog"), TEXT(""));

			}
			
		}
		else if(bIsProne){
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_run"), TEXT(""));

			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_prone_jog"), TEXT(""));

			}

		}
		else{
			
			if(bWalkPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_walk"), TEXT(""));

			}
			else if(bRunPressed){
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_run"), TEXT(""));
				
			}
			else{
				WalkSpeedRow = WalkSpeedTableObject->FindRow<FSTR_WalkSpeed>(FName("false_stand_jog"), TEXT(""));

			}
		}
	}

	if(WalkSpeedRow){
		if(MoveForwardAxis>0){
			WalkSpeed=WalkSpeedRow->Forward;
		}
		else{
			WalkSpeed=WalkSpeedRow->Other;
		}
		
		
	}

	//WalkSpeed perWeapon

	if(PlayerStateRef->GetHoldGun()){
		
		WalkSpeed=WalkSpeed*PlayerStateRef->GetHoldGun()->ItemWeaponRow->WalkSpeedPer;
	}
 
}


void ASCharacter::UpdateWeaponDisplay(FName HoldSocket){
	
	if(HoldSocket!=TEXT("None")){

		if(PlayerStateRef->GetHoldGun()){
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(HoldSocket);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			PlayerStateRef->GetHoldGun()->SetActorTransform(SocketTransform);
			PlayerStateRef->GetHoldGun()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),HoldSocket);

		}
	}
	bool bIsEquipBackpack;


	TArray<AItemBase*> GetEquipmentsx1=PlayerStateRef->GetEquipments();
	for(int32 i=0;i<GetEquipmentsx1.Num();i++){
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Backpack){
			bIsEquipBackpack=true;
		}
	}


	if(PlayerStateRef->GetWeapon1()){
		if(bIsEquipBackpack){
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackLeftBName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			PlayerStateRef->GetWeapon1()->SetActorTransform(SocketTransform);
			PlayerStateRef->GetWeapon1()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackLeftBName);

		}
		else{
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackLeftNName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			PlayerStateRef->GetWeapon1()->SetActorTransform(SocketTransform);
			PlayerStateRef->GetWeapon1()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackLeftNName);

		}
	}

	if(PlayerStateRef->GetWeapon2()){
		if(bIsEquipBackpack){
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackRightBName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			PlayerStateRef->GetWeapon2()->SetActorTransform(SocketTransform);
			PlayerStateRef->GetWeapon2()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackRightBName);

		}
		else{
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackRightNName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			PlayerStateRef->GetWeapon2()->SetActorTransform(SocketTransform);
			PlayerStateRef->GetWeapon2()->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackRightNName);

		}
	}

	
}




void ASCharacter::UpdateEquipmentDisplay(){
	TArray<AItemBase*> GetEquipmentsx1=PlayerStateRef->GetEquipments();
	for(int32 i=0;i<GetEquipmentsx1.Num();i++){
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Helmet){
			const USkeletalMeshSocket* GunSocket =GetMesh()->GetSocketByName(GameInstanceRef->HelmetName);
			const FTransform SocketTransform = GunSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->HelmetName);

		}
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Vest){
			const USkeletalMeshSocket* SKMSocket =GetMesh()->GetSocketByName(GameInstanceRef->VestName);
			const FTransform SocketTransform = SKMSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->VestName);

		}
		if(GetEquipmentsx1[i]->ItemType==E_ItemType::EIT_Backpack){
			const USkeletalMeshSocket* SKMSocket =GetMesh()->GetSocketByName(GameInstanceRef->BackpackName);
			const FTransform SocketTransform = SKMSocket->GetSocketTransform(GetMesh());
			GetEquipmentsx1[i]->SetActorTransform(SocketTransform);
			GetEquipmentsx1[i]->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative, true),GameInstanceRef->BackpackName);

		}
	}

	

}


FName ASCharacter::CalculateHoldGunSocket(){
	FName GunSocket;
	if(bIsProne){
		if(MoveForwardAxis==0&&MoveRightAxis==0){
			GunSocket=GameInstanceRef->GunProneIdleName;
		}
		else if(MoveRightAxis==0){
			GunSocket=GameInstanceRef->GunProneFBName;
		}
		else{
			GunSocket=GameInstanceRef->GunProneOtherName;
		}
	}
	else if(bIsCrouching&&!bIsAiming){
		GunSocket=GameInstanceRef->GunCrouchName;
	}
	else if(bIsAiming||bIsFiring){
		GunSocket=GameInstanceRef->GunAimName;
	}
	else if(bIsReload){
		GunSocket=GameInstanceRef->GunReloadName;
	}
	else{
		GunSocket=GameInstanceRef->GunStandName;
	}
	return GunSocket;
}


//ItemsOverlap


void ASCharacter::SetPickupItems(TArray<APickupBase*> Items){
	PickupItems=Items;
	for(int32 i=0;i<PickupItems.Num();i++){
		PickupItems[i]->OnBeginOverlap.AddDynamic(this,&ASCharacter::ExecBeginOverlap);
		PickupItems[i]->OnEndOverlap.AddDynamic(this,&ASCharacter::ExecEndOverlap);
	
	}
}

void ASCharacter::ExecBeginOverlap(APickupBase* PickupObject){
	
	//OutlineItem(PickupObject);
	ItemsInRange.Add(PickupObject);
}

void ASCharacter::ExecEndOverlap(APickupBase* PickupObject){
	
	PickupObject->EnabledOutLine(false);
	ItemsInRange.Remove(PickupObject);
}

void ASCharacter::OutlineItem(APickupBase* Item){
	for(int32 i=0;i<ItemsInRange.Num();i++){
		if(Item==ItemsInRange[i]){
			Item->EnabledOutLine(true);

		}
		else{
			Item->EnabledOutLine(false);
		}
	}
}

void ASCharacter::TargetingItem(){
	FHitResult OutHitResult;
	FVector OutHitLocation;
	float ShortestDistance=10000.f;
	APickupBase* ShortestItem;


	if(ItemsInRange.Num()>0){
			// Get Viewport Size
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get screen space location of crosshairs
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		// Get world position and direction of crosshairs
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection);

		if (bScreenToWorld){
			// Trace from Crosshair world location outward
			const FVector Start{ CrosshairWorldPosition };
			const FVector End{ Start + CrosshairWorldDirection * 500.f };
			OutHitLocation = End;
			GetWorld()->LineTraceSingleByChannel(
				OutHitResult,
				Start,
				End,
				ECollisionChannel::ECC_Visibility);
		}
		for(int32 i=0;i<ItemsInRange.Num();i++){
			ItemsInRange[i]->EnabledOutLine(false);
			if(FVector::Dist(ItemsInRange[i]->GetActorLocation(),OutHitResult.Location)<ShortestDistance){
				ShortestDistance=FVector::Dist(ItemsInRange[i]->GetActorLocation(),OutHitResult.Location);
				ShortestItem=ItemsInRange[i];
			}
		}
		if(ShortestItem){
			if(ShortestDistance<85.f)
			ShortestItem->EnabledOutLine(true);
			ReadyPickupItem=ShortestItem;

		}
		else{
			ReadyPickupItem=nullptr;
		}
	}

}


void ASCharacter::AutoPosition(E_WeaponPosition &Positionx,bool &bIsOnHandx){
	
	int32 CurrentAmount=0;
	if(PlayerStateRef->GetWeapon1()){
		CurrentAmount++;

	}
	if(PlayerStateRef->GetWeapon2()){
		CurrentAmount++;

	}
	if(PlayerStateRef->GetHoldGun()){
		CurrentAmount++;
	}
	if(CurrentAmount==0){
		Positionx=E_WeaponPosition::EWP_Left;
		bIsOnHandx=true;
		
	}
	else if(CurrentAmount<2){
		if(PlayerStateRef->GetHoldGun()){
			if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=false;
			}
			else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=false;
			}
		}
		else{
			if(PlayerStateRef->GetWeapon1()){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=false;
			}
			else{
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=false;

			}
		}
	}
	else{
		if(PlayerStateRef->GetHoldGun()){
			if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
				Positionx=E_WeaponPosition::EWP_Left;
				bIsOnHandx=true;
			}
			else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
				Positionx=E_WeaponPosition::EWP_Right;
				bIsOnHandx=true;
			}
		}
		else{
			Positionx=E_WeaponPosition::EWP_Left;
			bIsOnHandx=false;
		}
	}


}

void ASCharacter::AssignPosition(const E_WeaponPosition Assign,E_WeaponPosition &Positionx,bool &bIsOnHandx){

	if(PlayerStateRef->GetHoldGun()){
		if(PlayerStateRef->GetHoldGun()->Position==Assign){
			Positionx=Assign;
			bIsOnHandx=true;
		}
		else{
			Positionx=Assign;
			bIsOnHandx=false;
		}
	}
	else{
		Positionx=Assign;
		bIsOnHandx=false;
	}

}

void ASCharacter::SpawnPickupItem(AItemBase* ItemBasex1,APickupBase* &PickupItemx1){
	FName TempID=ItemBasex1->ID;
	FString TempSN=ItemBasex1->SN;
	FTransform TempTransform ;
	TempTransform.SetLocation(GetActorLocation());
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	if(ItemBasex1->ItemType==E_ItemType::EIT_Weapon){
		APickupWeapon* TempPickupWeapon=Cast<APickupWeapon>(ItemBasex1);
		TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeapon>(APickupWeapon::StaticClass(),TempTransform);
		if(TempPickupWeapon){
		
			TempPickupWeapon->ID=TempID;
			TempPickupWeapon->SN=TempSN;
			TempPickupWeapon->Amount=1;	
			AItemWeapon* TempWeapon=Cast<AItemWeapon>(ItemBasex1);
			
			TempPickupWeapon->Ammo=TempWeapon->Ammo;
			UGameplayStatics::FinishSpawningActor(TempPickupWeapon,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupWeapon);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Accessories){
		APickupWeaponAcc* TempPickupWeapon=Cast<APickupWeaponAcc>(ItemBasex1);
		TempPickupWeapon=GetWorld()->SpawnActorDeferred<APickupWeaponAcc>(APickupWeaponAcc::StaticClass(),TempTransform);
		if(TempPickupWeapon){
			TempPickupWeapon->ID=TempID;
			TempPickupWeapon->SN=TempSN;
			TempPickupWeapon->Amount=1;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupWeapon,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupWeapon);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Ammo){
		APickupAmmo* TempPickupBase=Cast<APickupAmmo>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupAmmo>(APickupAmmo::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Health){
		APickupHealth* TempPickupBase=Cast<APickupHealth>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupHealth>(APickupHealth::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Boost){
		APickupBoost* TempPickupBase=Cast<APickupBoost>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupBoost>(APickupBoost::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=ItemBasex1->Amount;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}

	else if(ItemBasex1->ItemType==E_ItemType::EIT_Helmet||ItemBasex1->ItemType==E_ItemType::EIT_Vest||ItemBasex1->ItemType==E_ItemType::EIT_Backpack){
		APickupEquipment* TempPickupBase=Cast<APickupEquipment>(ItemBasex1);
		TempPickupBase=GetWorld()->SpawnActorDeferred<APickupEquipment>(APickupEquipment::StaticClass(),TempTransform);
		if(TempPickupBase){
			TempPickupBase->ID=TempID;
			TempPickupBase->SN=TempSN;
			TempPickupBase->Amount=1;	
			
			
			UGameplayStatics::FinishSpawningActor(TempPickupBase,TempTransform);
		}
		PickupItemx1=Cast<APickupBase>(TempPickupBase);

	}
	
	
	CompleteSpawnPickupItem(PickupItemx1);
	
	
	

}

void ASCharacter::CompleteSpawnPickupItem(APickupBase* PickupItemx1){
	PickupItems.Add(PickupItemx1);
	PickupItemx1->OnBeginOverlap.AddDynamic(this,&ASCharacter::ExecBeginOverlap);
	PickupItemx1->OnEndOverlap.AddDynamic(this,&ASCharacter::ExecEndOverlap);
	PickupItemx1->GetBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupItemx1->GetBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void ASCharacter::BeginDiscard(){
	if(!bIsProne){
		if(!bIsPlayingMontage){
			if(PlayerStateRef->GetHoldGun()){
				DiscardWeapon(PlayerStateRef->GetHoldGun());
			}
			else if(PlayerStateRef->GetWeapon1()){
				DiscardWeapon(PlayerStateRef->GetWeapon1());
			}
			else if (PlayerStateRef->GetWeapon2()){
				DiscardWeapon(PlayerStateRef->GetWeapon2());
			}
		}
	}
}



void ASCharacter::DiscardWeapon(AItemWeapon* ItemWeaponx1){
	FName TempIDName;
	if(ItemWeaponx1->Ammo>0){
		TempIDName=ItemWeaponx1->ItemWeaponRow->UseAmmoID;
		AItemAmmo* TempAmmo;
		FTransform TempTransform;
		TempTransform.SetLocation(FVector::ZeroVector);
		TempTransform.SetRotation(FQuat(0,0,0,0));
		TempTransform.SetScale3D(FVector(1.f,1.f,1.f));

		TempAmmo=GetWorld()->SpawnActorDeferred<AItemAmmo>(AItemAmmo::StaticClass(),TempTransform);
		if(TempAmmo){
			
			TempAmmo->ID=TempIDName;
			TempAmmo->SN=GameInstanceRef->GenrateSN();
			TempAmmo->Amount=ItemWeaponx1->Ammo;	
			
			
			UGameplayStatics::FinishSpawningActor(TempAmmo,TempTransform);
		}
		APickupBase* TempPickupBasex1;
		SpawnPickupItem(TempAmmo,TempPickupBasex1);
		ItemWeaponx1->Ammo=0;
	}
	APickupBase* TempPickupBase;
	SpawnPickupItem(ItemWeaponx1,TempPickupBase);
	if (ItemWeaponx1->SightAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->SightAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->SightAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->ForegripAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->ForegripAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->ForegripAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->MagAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->MagAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->MagAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->MuzzleAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->MuzzleAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->MuzzleAccActorx1)->Destroy();
		
	}
	if (ItemWeaponx1->ButtstockAccActorx1)
	{
		AItemWeaponAcc* TempItemBas=ItemWeaponx1->ButtstockAccActorx1;
		AItemBase* TempItemBase=Cast<AItemBase>(TempItemBas);
		
		SpawnPickupItem(TempItemBase,TempPickupBase);
		bool bIsDestroyed=Cast<AActor>(ItemWeaponx1->ButtstockAccActorx1)->Destroy();
		
	}
	if(ItemWeaponx1->bIsOnHand){
		PlayerStateRef->SetHoldGun(nullptr);
	}
	else{
		if(ItemWeaponx1->Position==E_WeaponPosition::EWP_Left){
			PlayerStateRef->SetWeapon1(nullptr);
		}
		else if(ItemWeaponx1->Position==E_WeaponPosition::EWP_Right){
			PlayerStateRef->SetWeapon2(nullptr);

		}
	} 
	bool bIsDestroyed=Cast<AActor>(ItemWeaponx1)->Destroy();
		
}

void ASCharacter::PickupWeapon(class APickupWeapon* PickupWeaponx1,bool bIsAssign,E_WeaponPosition Positionx1){
	E_WeaponPosition TargetPosition;
	bool TargetbIsOnHand;
	AItemWeapon* ReplaceWeapon=nullptr;
	if(bIsAssign){
		AssignPosition(Positionx1,TargetPosition,TargetbIsOnHand);
	}
	else{
		AutoPosition(TargetPosition,TargetbIsOnHand);
	}
	
	
	
	if(TargetbIsOnHand){
		if(PlayerStateRef->GetHoldGun()){
			ReplaceWeapon=PlayerStateRef->GetHoldGun();
		}
	}
	else{
		if(TargetPosition==E_WeaponPosition::EWP_Left){
			if(PlayerStateRef->GetWeapon1()){
				ReplaceWeapon=PlayerStateRef->GetWeapon1();
			}

		}
		else if(TargetPosition==E_WeaponPosition::EWP_Right){
			if(PlayerStateRef->GetWeapon2()){
				ReplaceWeapon=PlayerStateRef->GetWeapon2();
			}
			
		}
	}
	//NextStep
	if(ReplaceWeapon!=nullptr){
		
		DiscardWeapon(ReplaceWeapon);
	}
	AItemWeapon* TempWeapon;
	FTransform TempTransform;
	TempTransform.SetLocation(FVector::ZeroVector);
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	TempWeapon=GetWorld()->SpawnActorDeferred<AItemWeapon>(AItemWeapon::StaticClass(),TempTransform);
	if(TempWeapon){
		
		TempWeapon->Ammo=PickupWeaponx1->Ammo;
		TempWeapon->Position=TargetPosition;
		TempWeapon->ID=PickupWeaponx1->ID;
		TempWeapon->bIsOnHand=TargetbIsOnHand;
		TempWeapon->SN=PickupWeaponx1->SN;
		TempWeapon->Amount=1;	
		
		
		UGameplayStatics::FinishSpawningActor(TempWeapon,TempTransform);
	}
	if(TargetbIsOnHand){
		PlayerStateRef->SetHoldGun(TempWeapon);
	}
	else{
		if(TargetPosition==E_WeaponPosition::EWP_Left){
			PlayerStateRef->SetWeapon1(TempWeapon);
		}
		else if(TargetPosition==E_WeaponPosition::EWP_Right){
			PlayerStateRef->SetWeapon2(TempWeapon);
		}
	}
	

}

void ASCharacter::BeginPickupItem(){
	
	bool bIsSuccess=PickupItem();
}



bool ASCharacter::PickupItem(){
	
	if(ReadyPickupItem){
		if(ReadyPickupItem->ItemType==E_ItemType::EIT_Weapon){
			APickupWeapon* TempWeapon=Cast<APickupWeapon>(ReadyPickupItem);
			bool TempbIsAssign=false;
			E_WeaponPosition TempPosition=E_WeaponPosition::EWP_Left;
			PickupWeapon(TempWeapon,TempbIsAssign,TempPosition);
			bool bIsSuccess=Cast<AActor>(ReadyPickupItem)->Destroy();
			return true;
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Accessories){
			APickupWeaponAcc* TempWeaponAcc=Cast<APickupWeaponAcc>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempWeaponAcc);
			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx1=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;

			}
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Ammo){
			APickupAmmo* TempAmmo=Cast<APickupAmmo>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempAmmo);
			
			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx2=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
				
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Health){
			APickupHealth* TempHealth=Cast<APickupHealth>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempHealth);

			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx3=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Boost){
			APickupBoost* TempBoost=Cast<APickupBoost>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempBoost);


			if(PickupGoods(TempPickupBase)){
				bool bIsSuccessx4=Cast<AActor>(ReadyPickupItem)->Destroy();
				return true;
			}
			else{
				return false;
				
			}
		}
		else if(ReadyPickupItem->ItemType==E_ItemType::EIT_Helmet||
		ReadyPickupItem->ItemType==E_ItemType::EIT_Vest||
		ReadyPickupItem->ItemType==E_ItemType::EIT_Backpack){
			APickupEquipment* TempEquipment=Cast<APickupEquipment>(ReadyPickupItem);
			APickupBase* TempPickupBase=Cast<APickupBase>(TempEquipment);

			bool bIsSucceed=PickupEquipment(TempPickupBase);
			bool bIsSuccessx3=Cast<AActor>(ReadyPickupItem)->Destroy();
			if(!bIsSucceed){
				return false;
			}
			return true;
		}
		
		
		
		
	}
	else{
		return false;
	}
	
	return false;
}



bool ASCharacter::PickupGoods(APickupBase* PickupBasex1){
	if(PlayerStateRef->CheckBackpackCapacity(PickupBasex1->GetWeight())){
		
		if(PickupBasex1->ItemType==E_ItemType::EIT_Accessories){
			AItemWeaponAcc* TempWeaponAcc;
				FTransform TempTransform;
				TempTransform.SetLocation(FVector::ZeroVector);
				TempTransform.SetRotation(FQuat(0,0,0,0));
				TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
				TempWeaponAcc=GetWorld()->SpawnActorDeferred<AItemWeaponAcc>(AItemWeaponAcc::StaticClass(),TempTransform);
				if(TempWeaponAcc){
					
					
					TempWeaponAcc->ID=PickupBasex1->ID;
					
					TempWeaponAcc->SN=PickupBasex1->SN;
					TempWeaponAcc->Amount=1;	
					
					
					UGameplayStatics::FinishSpawningActor(TempWeaponAcc,TempTransform);
				}
				AItemBase* TempItemBase=Cast<AItemBase>(TempWeaponAcc);
				PlayerStateRef->AddItemsInBackpack(TempItemBase);
		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Ammo){
			PlayerStateRef->UpdateAmmoAmount(PickupBasex1->ID,true,PickupBasex1->Amount);

		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Health){
			AItemHealth* TempHealth;
			FTransform TempTransform;
			TempTransform.SetLocation(FVector::ZeroVector);
			TempTransform.SetRotation(FQuat(0,0,0,0));
			TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
			TempHealth=GetWorld()->SpawnActorDeferred<AItemHealth>(AItemHealth::StaticClass(),TempTransform);
			if(TempHealth){
				
				
				TempHealth->ID=PickupBasex1->ID;
				
				TempHealth->SN=PickupBasex1->SN;
				TempHealth->Amount=PickupBasex1->Amount;	
				
				
				UGameplayStatics::FinishSpawningActor(TempHealth,TempTransform);
			}
			AItemBase* TempItemBase=Cast<AItemBase>(TempHealth);
			PlayerStateRef->AddItemsInBackpack(TempItemBase);
		}
		else if(PickupBasex1->ItemType==E_ItemType::EIT_Boost){
			AItemBoost* TempBoost;
			FTransform TempTransform;
			TempTransform.SetLocation(FVector::ZeroVector);
			TempTransform.SetRotation(FQuat(0,0,0,0));
			TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
			TempBoost=GetWorld()->SpawnActorDeferred<AItemBoost>(AItemBoost::StaticClass(),TempTransform);
			if(TempBoost){
				
				
				TempBoost->ID=PickupBasex1->ID;
				
				TempBoost->SN=PickupBasex1->SN;
				TempBoost->Amount=PickupBasex1->Amount;	
				
				
				UGameplayStatics::FinishSpawningActor(TempBoost,TempTransform);
			}
			AItemBase* TempItemBasex1=Cast<AItemBase>(TempBoost);
			PlayerStateRef->AddItemsInBackpack(TempItemBasex1);
		}
		
		return true;

	}
	else{
		UE_LOG(LogTemp,Warning,TEXT("NotEnoughCapacity"));
		return false;
	}
	return false;
}





void ASCharacter::UpdateCharacterGunState(){

	if(PlayerStateRef->GetHoldGun()){
		bIsHoldWeapon=true;
	}
	else{
		bIsHoldWeapon=false;
	}



}

void ASCharacter::PlayMontage(E_MontageType MontageType){
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	PlayingMontageType=MontageType;
	bIsPlayingMontage=true;
	if(bIsProne){
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && ProneEquipMontage)
				{
					AnimInstance->Montage_Play(ProneEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && ProneUnEquipMontage)
				{
					AnimInstance->Montage_Play(ProneUnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && ProneReloadMontage)
				{
					AnimInstance->Montage_Play(ProneReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && ProneReloadMontage)
				{
					AnimInstance->Montage_Play(ProneReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && ProneFireMontage)
				{
					AnimInstance->Montage_Play(ProneFireMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && ProneUseMontage)
				{
					AnimInstance->Montage_Play(ProneUseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}
	else if(bIsCrouching){
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && CrouchEquipMontage)
				{
					AnimInstance->Montage_Play(CrouchEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && CrouchUnEquipMontage)
				{
					AnimInstance->Montage_Play(CrouchUnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && CrouchReloadMontage)
				{
					AnimInstance->Montage_Play(CrouchReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && CrouchReloadMontage)
				{
					AnimInstance->Montage_Play(CrouchReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && CrouchFireMontage)
				{
					AnimInstance->Montage_Play(CrouchFireMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && CrouchUseMontage)
				{
					AnimInstance->Montage_Play(CrouchUseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}
	else{
		switch (MontageType)
		{
			case E_MontageType::EMT_Equip:
				if (AnimInstance && EquipMontage)
				{
					AnimInstance->Montage_Play(EquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_UnEquip:
				if (AnimInstance && UnEquipMontage)
				{
					AnimInstance->Montage_Play(UnEquipMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_Reload:
				if (AnimInstance && ReloadMontage)
				{
					AnimInstance->Montage_Play(ReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;

			case E_MontageType::EMT_ReloadBullet:
				if (AnimInstance && ReloadMontage)
				{
					AnimInstance->Montage_Play(ReloadMontage);
					AnimInstance->Montage_JumpToSection(FName("ReloadBullet"));
				}
			break;
			case E_MontageType::EMT_Fire:
				if (AnimInstance && FireMontage)
				{
					AnimInstance->Montage_Play(FireMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;
			case E_MontageType::EMT_Use:
				if (AnimInstance && UseMontage)
				{
					AnimInstance->Montage_Play(UseMontage);
					AnimInstance->Montage_JumpToSection(FName("Default"));
				}
			break;


			
			
		}
	}


}

void ASCharacter::BeginPlayMontage(){
	if(PlayerStateRef->GetHoldGun()){
		PlayMontage(E_MontageType::EMT_UnEquip);
	}
}

void ASCharacter::TakeBackWeapon(){
	bIsHoldWeapon=false;
	PlayerStateRef->GetHoldGun()->bIsOnHand=false;
	if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Left){
		PlayerStateRef->SetWeapon1(PlayerStateRef->GetHoldGun());
		PlayerStateRef->SetHoldGun(nullptr);
	}
	else if(PlayerStateRef->GetHoldGun()->Position==E_WeaponPosition::EWP_Right){
		PlayerStateRef->SetWeapon2(PlayerStateRef->GetHoldGun());
		PlayerStateRef->SetHoldGun(nullptr);
	}

}

void ASCharacter::Keyboard1KeyPressed(){
	ReadyEquipWeapon =PlayerStateRef->GetWeapon1();
	if(ReadyEquipWeapon){
		if(PlayerStateRef->GetHoldGun()){
			PlayMontage(E_MontageType::EMT_UnEquip);
		}
		else{
			PlayMontage(E_MontageType::EMT_Equip);
		}
	}
}

void ASCharacter::Keyboard2KeyPressed(){
	ReadyEquipWeapon =PlayerStateRef->GetWeapon2();
	if(ReadyEquipWeapon){
		if(PlayerStateRef->GetHoldGun()){
			PlayMontage(E_MontageType::EMT_UnEquip);
		}
		else{
			PlayMontage(E_MontageType::EMT_Equip);
		}
	}
}


void ASCharacter::EquipWeapon(){
	bIsHoldWeapon=true;
	PlayerStateRef->SetHoldGun(ReadyEquipWeapon);
	ReadyEquipWeapon->bIsOnHand=true;
	if(ReadyEquipWeapon->Position==E_WeaponPosition::EWP_Left){
		PlayerStateRef->SetWeapon1(nullptr);
		ReadyEquipWeapon=nullptr;
	}
	else if(ReadyEquipWeapon->Position==E_WeaponPosition::EWP_Right){
		PlayerStateRef->SetWeapon2(nullptr);
		ReadyEquipWeapon=nullptr;
	}

}

void ASCharacter::DiscardItem(AItemBase* Itemx1){
	APickupBase* PickupBasex1;
	SpawnPickupItem(Itemx1,PickupBasex1);

	if(Itemx1->ItemType==E_ItemType::EIT_Ammo){	
		PlayerStateRef->UpdateAmmoAmount(Itemx1->ID,false,0);
		
		

	}
	if(Itemx1){
		PlayerStateRef->RemoveItemsInBackpack(Itemx1);
		bool bIsDestroyed=Cast<AActor>(Itemx1)->Destroy();
	
	}
}

bool ASCharacter::DiscardEquipment(AItemBase* Itemx1,bool bIsCheck){
	if(bIsCheck){

	
		if(Itemx1->ItemType==E_ItemType::EIT_Backpack){
			if(PlayerStateRef->CheckReplaceBackpack(Itemx1)){

			}
			else{
				UE_LOG(LogTemp,Warning,TEXT("Cannot DiscardBackpack"));
				return false;
			}
			

		}
	}
	APickupBase* TempPickupBase;
	SpawnPickupItem(Itemx1,TempPickupBase);
	PlayerStateRef->RemoveEquipment(Itemx1);

	bool bIsDestroyed=Cast<AActor>(Itemx1)->Destroy();
	return true;
}

bool ASCharacter::PickupEquipment(APickupBase* PickupBasex1){

	if(PickupBasex1->ItemType==E_ItemType::EIT_Backpack){
		if(PlayerStateRef->CheckReplaceBackpack(PickupBasex1)){

		}
		else{
			UE_LOG(LogTemp,Warning,TEXT("Cannot PickupBackpack"));

		}
	}

	//NextSteps
	AItemBase* CurrentItem=nullptr;
	TArray<AItemBase*> OwnEquipments=PlayerStateRef->GetEquipments();
	for(int32 i=0;i<OwnEquipments.Num();i++){
		if(OwnEquipments[i]->ItemType==PickupBasex1->ItemType){
			CurrentItem=OwnEquipments[i];
		}
	}


	if(CurrentItem){
		bool bIsSucceed= DiscardEquipment(CurrentItem,false);
	}

	AItemEquipment* TempEquipment;
	FTransform TempTransform;
	TempTransform.SetLocation(FVector::ZeroVector);
	TempTransform.SetRotation(FQuat(0,0,0,0));
	TempTransform.SetScale3D(FVector(1.f,1.f,1.f));
	TempEquipment=GetWorld()->SpawnActorDeferred<AItemEquipment>(AItemEquipment::StaticClass(),TempTransform);
	if(TempEquipment){
		
		
		TempEquipment->ID=PickupBasex1->ID;
		
		TempEquipment->SN=PickupBasex1->SN;
		TempEquipment->Amount=PickupBasex1->Amount;	
		
		
		UGameplayStatics::FinishSpawningActor(TempEquipment,TempTransform);
	}
	PlayerStateRef->AddEquipment(TempEquipment);
	return true;

}





//ASCharacter

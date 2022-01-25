// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ItemWeapon.h"
#include "PickupBase.h"
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









//ASCharacter

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "ItemBase.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UDataTable;
class APickupBase;
class ASPlayerState;
class UPUBGAdvancedTutGI;
class AItemWeapon;



USTRUCT(BlueprintType)
struct FSTR_ProneTime : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sec;

	
};

USTRUCT(BlueprintType)
struct FSTR_WalkSpeed : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Other;
	

	
};

UENUM(BlueprintType)
enum class E_MontageType : uint8
{
	EMT_Equip UMETA(DisplayName = "Equip"),
	EMT_UnEquip UMETA(DisplayName = "UnEquip"),
	EMT_Reload UMETA(DisplayName = "Reload"),
	EMT_ReloadBullet UMETA(DisplayName = "ReloadBullet"),
	EMT_Fire UMETA(DisplayName = "Fire"),
	EMT_Use UMETA(DisplayName = "Use")

	
};


UCLASS()
class PUBGADVANCEDTUT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	UFUNCTION(BlueprintCallable,Category=ItemsOverlap)
	void SetPickupItems(TArray<APickupBase*> Items);
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void SmoothIncrease();

	void AltKeyPressed();

	void AltKeyReleased();

	void CrouchKeyPressed();

	void ProneKeyPressed();

	void JumpKeyPressed();

	void Turn(float Value);

	void LookUp(float Value);

	void UpdateCameraHeight();

	void WalkKeyPressed();

	void WalkKeyReleased();

	void RunKeyPressed();

	void RunKeyReleased();

	void AimKeyPressed();

	void AimKeyReleased();



	UFUNCTION(BlueprintImplementableEvent)
	void HandleAltKeyReleased(FRotator AltKeyPressedRotationx,FRotator AltKeyReleasedRotationx);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleCameraHeightChanged(float OriginalHeight,float NewHeight);

	
	void HandleProneTimeFromTable(int32 Pose1,int32 Pose2);

	void HandleWalkSpeedFromTable();

	UFUNCTION()
	void EnableMove();

	bool LimitPitchAngle(float Axis);

	//ChangingItems
	UFUNCTION(BlueprintCallable,Category=ChangingWeaponDisplay)
	void UpdateWeaponDisplay(FName HoldSocket);

	UFUNCTION(BlueprintCallable,Category=ChangingEquipmentDisplay)
	void UpdateEquipmentDisplay();

	//Items->SocketClaculate

	UFUNCTION(BlueprintPure,Category=CalculateSocket)
	FName CalculateHoldGunSocket();

	//Items Overlap

	
	UFUNCTION()
	void ExecBeginOverlap(APickupBase* PickupObject);

	UFUNCTION()
	void ExecEndOverlap(APickupBase* PickupObject);

	void OutlineItem(APickupBase* Item);

	UFUNCTION(BlueprintCallable)
	void TargetingItem();
	
	//Items->PickupWeapon
	void AutoPosition(E_WeaponPosition &Positionx,bool &bIsOnHandx);

	void AssignPosition(const E_WeaponPosition Assign,E_WeaponPosition &Positionx,bool &bIsOnHandx);

	//Items->Discard
	UFUNCTION(BlueprintCallable)
	void SpawnPickupItem(AItemBase* ItemBasex1,APickupBase* &PickupItemx1);

	void CompleteSpawnPickupItem(APickupBase* PickupItemx1);

	void BeginDiscard();

	void DiscardItem(AItemBase* Itemx1);

	bool DiscardEquipment(AItemBase* Itemx1,bool bIsCheck);

	//Items->Pickup
	void BeginPickupItem();

	void DiscardWeapon(AItemWeapon* ItemWeaponx1);

	void PickupWeapon(class APickupWeapon* PickupWeaponx1,bool bIsAssign,E_WeaponPosition Position1);

	bool PickupItem();

	bool PickupGoods(APickupBase* PickupBasex1);

	bool PickupEquipment(APickupBase* PickupBasex1);


	//Play Animation
	UFUNCTION(BlueprintCallable)
	void PlayMontage(E_MontageType MontageType);
	
	void BeginPlayMontage();
	
	//WeaponChanged

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterGunState();

	UFUNCTION(BlueprintCallable)
	void TakeBackWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	void Keyboard1KeyPressed();

	void Keyboard2KeyPressed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed=600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator AltKeyPressedRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator AltKeyReleasedRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FRotator CurrentRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAltKeyPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AltReleaseRotationRate=.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterPose, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterPose, meta = (AllowPrivateAccess = "true"))
	bool bIsProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsHoldWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState, meta = (AllowPrivateAccess = "true"))
	bool bIsReload;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OnTheAir, meta = (AllowPrivateAccess = "true"))
	bool bIsOnTheAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OnTheAir, meta = (AllowPrivateAccess = "true"))
	bool bIsFreeFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OnTheAir, meta = (AllowPrivateAccess = "true"))
	bool bIsUmbrellaOpened;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OnTheAir, meta = (AllowPrivateAccess = "true"))
	float ForwardValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsPlayingMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bEnableMove=true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ProneEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ProneUnEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ProneReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ProneFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ProneUseMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CrouchEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CrouchUnEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CrouchReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CrouchFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CrouchUseMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UnEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UseMontage;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_ProneTime;

	FString ProneTimeTablePath;

	UDataTable* ProneTimeTableObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_WalkSpeed;

	FString WalkSpeedTablePath;

	UDataTable* WalkSpeedTableObject;

	

	FTimerHandle TH_DisableMove;

	float DisableMoveTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float MouseAimTurnRate=.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float MouseAimLookUpRate=.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float MouseHipTurnRate=1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float MouseHipLookUpRate=1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float ProneBottomValue=350.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float ProneTopValue=15.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float CrouchBottomValue=290.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float CrouchTopValue=40.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float StandBottomValue=330.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mouse, meta = (AllowPrivateAccess = "true"))
	float StandTopValue=45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float StandCameraHeight=60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchCameraHeight=40.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchRunCameraHeight=45.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchRifleMoveCameraHeight=50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ProneCameraHeight=10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float MoveRightAxis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, meta = (AllowPrivateAccess = "true"))
	float MoveForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CurrentCameraHeight=60.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float NewCameraHeight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float OriginalCameraHeight=60.f;

	bool bWalkPressed;

	bool bRunPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	AItemBase* ItemBaseRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemBase> DefaultItemBaseClass;
	
	TArray<APickupBase*> PickupItems;

	TArray<APickupBase*> ItemsInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState, meta = (AllowPrivateAccess = "true"))
	ASPlayerState* PlayerStateRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameInstance, meta = (AllowPrivateAccess = "true"))
	UPUBGAdvancedTutGI* GameInstanceRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState, meta = (AllowPrivateAccess = "true"))
	APickupBase* ReadyPickupItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerAnimState, meta = (AllowPrivateAccess = "true"))
	E_MontageType PlayingMontageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	AItemWeapon* ReadyEquipWeapon;


public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetIsCrouching() const {return bIsCrouching;}
	FORCEINLINE bool GetIsProne() const {return bIsProne;}
	FORCEINLINE bool GetIsDead() const {return bIsDead;}
	FORCEINLINE bool GetIsHoldWeapon() const {return bIsHoldWeapon;}
	FORCEINLINE bool GetIsAiming() const {return bIsAiming;}
	FORCEINLINE bool GetIsFiring() const {return bIsFiring;}
	FORCEINLINE bool GetIsEquip() const {return bIsEquip;}
	FORCEINLINE bool GetIsReload() const {return bIsReload;}
	FORCEINLINE bool GetIsOnTheAir() const {return bIsOnTheAir;}
	FORCEINLINE bool GetIsFreeFalling() const {return bIsFreeFalling;}
	FORCEINLINE bool GetIsUmbrellaOpened() const {return bIsUmbrellaOpened;}
	FORCEINLINE bool GetIsPlayingMontage() const {return bIsPlayingMontage;}

	FORCEINLINE float GetForwardValue() const {return ForwardValue;}

	FRotator GetControllerxRotation();


};

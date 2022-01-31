// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "ItemBase.generated.h"


class ASCharacter;

USTRUCT(BlueprintType)
struct FSTR_ItemWeapon : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* BigIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UseAmmoID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FiringInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReplaceClipTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReplaceBulletTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DefaultButtstock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireSoundSuppressor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanButtstock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanGrip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMinStillness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMaxStillness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMinMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMaxMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMinAimStillness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMaxAimStillness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMinAimMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrossOffsetMaxAimMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HundredMVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThreeHundredMVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FiveHundredMVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedPer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent;
	
};

USTRUCT(BlueprintType)
struct FSTR_ItemAmmo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PickupAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent;
};

UENUM(BlueprintType)
enum class E_WeaponAccType : uint8
{
	EWAT_Sight UMETA(DisplayName = "Sight"),
	EWAT_Muzzle UMETA(DisplayName = "Muzzle"),
	EWAT_Foregrip UMETA(DisplayName = "Foregrip"),
	EWAT_Mag UMETA(DisplayName = "Mag"),

	EWAT_Buttstock UMETA(DisplayName = "Buttstock")
};

USTRUCT(BlueprintType)
struct FSTR_ItemWeaponAcc : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_WeaponAccType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> WeaponIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SightMultiple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSuppressor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsNotFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClipRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ClipCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalAdjPer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalAdjPer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	
};

UENUM(BlueprintType)
enum class E_ItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Accessories UMETA(DisplayName = "Accessories"),
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Health UMETA(DisplayName = "Health"),
	EIT_Boost UMETA(DisplayName = "Boost"),
	EIT_Helmet UMETA(DisplayName = "Helmet"),
	EIT_Vest UMETA(DisplayName = "Vest"),
	EIT_Backpack UMETA(DisplayName = "Backpack")

	
};





USTRUCT(BlueprintType)
struct FSTR_ItemHealth : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Limit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent;
};

USTRUCT(BlueprintType)
struct FSTR_ItemEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_ItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent;
};

USTRUCT(BlueprintType)
struct FSTR_ItemBoost : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BoostValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProbabilityPercent;

	
};

USTRUCT(BlueprintType)
struct FSTR_NumberProbability : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Percentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number;

	
	
};

USTRUCT(BlueprintType)
struct FSTR_ItemTypeProbability : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Percentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_ItemType Type;

};


UENUM(BlueprintType)
enum class E_WeaponPosition : uint8
{
	EWP_Left UMETA(DisplayName = "Left"),
	EWP_Right UMETA(DisplayName = "Right")
	
	
};


UENUM(BlueprintType)
enum class E_ShootMode : uint8
{
	ESM_Single UMETA(DisplayName = "Single"),
	ESM_Auto UMETA(DisplayName = "Auto"),
	
	
};


USTRUCT(BlueprintType)
struct FGate
{
	GENERATED_BODY()
public:
	FORCEINLINE void Open() {bIsGateOpen=true; }

	FORCEINLINE void Close() {bIsGateOpen=false; }

	FORCEINLINE void Toggle() {bIsGateOpen=!bIsGateOpen; }

	FORCEINLINE bool IsGateOpen() const {return bIsGateOpen; }

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsGateOpen=true;
	

};



UCLASS()
class PUBGADVANCEDTUT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	ASCharacter* CharacterRef;

	void Init(E_ItemType ItemTypex1,FText Namex1);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	USceneComponent* SceneComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	E_ItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item,meta=(ExposeOnSpawn="true"))
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item,meta=(ExposeOnSpawn="true"))
	FString SN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item,meta=(ExposeOnSpawn="true"))
	int32 Amount;

	//FORCEINLINE void SetCharacter(ASCharacter* Char) { CharacterRef = Char; }

	virtual int32 GetWeight();

	void UpdateAmount(bool bIsAdd,bool bIsReduce,int32 Amount1);


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* DT_ItemWeapon;

	FString ItemWeaponTablePath;

	UDataTable* ItemWeaponTableObject;

	

	

};

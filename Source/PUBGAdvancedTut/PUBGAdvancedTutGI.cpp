// Fill out your copyright notice in the Description page of Project Settings.


#include "PUBGAdvancedTutGI.h"



UPUBGAdvancedTutGI::UPUBGAdvancedTutGI(){


    
}

FString UPUBGAdvancedTutGI::GenrateSN(){
    FString RandFlt=FString::SanitizeFloat(FMath::RandRange(0.f,2000.f));
    FString RandFlt2=FString::SanitizeFloat(FMath::RandRange(0.f,2000.f));
    
    return RandFlt.Append(RandFlt2);
    
}

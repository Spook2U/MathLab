// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathLabLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MATHLAB_API UMathLabLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
   //UMathLabLibrary(const FObjectInitializer& ObjectInitializer);   

   ///* This method is only working in Debug or Development Mode */
   //UFUNCTION(BlueprintCallable, DisplayName = "Print (Debug)", Category = "Math Lab|Utilities|String", meta = (Keywords = "mld, print, debug, printdebug"))
   //static void bp_debug_Screen(FString inString = "Hello", FLinearColor color = FLinearColor::White);

   /* Converts a n-vector value to a string, in the form '{x, y, z, ...}' */
   //UFUNCTION(BlueprintPure, DisplayName = "ToString (nVector)", Category = "Math Lab|nVector", meta = (CompactNodeTitle = "->", BlueprintAutocast))
   //static FString Conv_NVectorToString(FNVector InVec);


	
	
	
};

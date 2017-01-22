// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "NVector.generated.h"

UCLASS()
class MATHLAB_API UNVector : public UObject
{
   GENERATED_BODY()
   UNVector();
   // Unreal Callbacks---------------------------------------------------------------------------------

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   TArray<float> Coordinates;

private:

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Creates a nVector instance
   UFUNCTION(BlueprintPure, Category = "nVector")
   static UNVector *MakeNVector(TArray<float> coordinates);

   //Converts the nVector into a FString format
   UFUNCTION(BlueprintPure, Category = "nVector")
   FString ToString();



   // -------------------------------------------------------------------------------------------------
protected:
   void Init(TArray<float> coordinates);
};

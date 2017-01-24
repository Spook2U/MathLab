// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "NVector.generated.h"

UCLASS()
class MATHLAB_API UNVector : public UObject
{
   GENERATED_BODY()

public:
   UNVector();
   // Unreal Callbacks---------------------------------------------------------------------------------

public:
   TArray<float> Coordinates;

private:

public:
   //Converts the nVector into a FString format
   UFUNCTION(BlueprintPure, Category = "nVector")
   FString ToString();

   void Init(TArray<float> coordinates);

protected:
};

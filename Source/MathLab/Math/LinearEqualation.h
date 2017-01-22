// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Math/NMatrix.h"
#include "LinearEqualation.generated.h"

UCLASS()
class MATHLAB_API ULinearEqualation : public UObject
{
	GENERATED_BODY()
	
public:
   ULinearEqualation();
   // Unreal Callbacks---------------------------------------------------------------------------------

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   TArray<UNVector *> CoefficientMatrix;

   UNMatrix *testMatrix;

   int NumColumns;
   int NumRows;

private:

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Creates a linear Equalation instance
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   static ULinearEqualation *MakeLinearEqualation(TArray<UNVector *> coefficientMatrix);

   //Solves the linear Equalation
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   UNVector *Solve();

   //Converts the linear equalation into a FString format
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   FString ToString();

  

   // -------------------------------------------------------------------------------------------------
private:
   void Init(TArray<UNVector *> coefficientMatrix);
};

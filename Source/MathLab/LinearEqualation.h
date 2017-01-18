// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "NVector.h"
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

   //Returns the element of the coefficient Matrix within the column and row
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   float GetElement(int column, int row);

   //Returns a nVector containing the values of requested column
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   UNVector *GetColumn(int column);

   //Returns a nVector containing the values of requested row
   UFUNCTION(BlueprintPure, Category = "Linear Equalation")
   UNVector *GetRow(int row);

   

// -------------------------------------------------------------------------------------------------
private:
   void Init(TArray<UNVector *> coefficientMatrix);
};

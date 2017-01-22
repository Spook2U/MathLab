// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Math/NVector.h"
#include "NMatrix.generated.h"

UCLASS()
class MATHLAB_API UNMatrix : public UObject
{
	GENERATED_BODY()
	UNMatrix();
   // Unreal Callbacks---------------------------------------------------------------------------------

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   TArray<UNVector *> Matrix;

private:

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   //Creates a nVector instance
   UFUNCTION(BlueprintPure, Category = "nMatrix")
   static UNMatrix *MakeNMatrix(TArray<UNVector *> matrix);

   //Converts the nVector into a FString format
   UFUNCTION(BlueprintPure, Category = "nMatrix")
   FString ToString();

   // -------------------------------------------------------------------------------------------------

   //Returns the element of the coefficient Matrix within the column and row
   UFUNCTION(BlueprintPure, Category = "nMatrix")
   float GetElement(int column, int row);

   //Returns a nVector containing the values of requested column
   UFUNCTION(BlueprintPure, Category = "nMatrix")
   UNVector *GetColumn(int column);

   //Returns a nVector containing the values of requested row
   UFUNCTION(BlueprintPure, Category = "nMatrix")
   UNVector *GetRow(int row);

   // -------------------------------------------------------------------------------------------------

   UFUNCTION(BlueprintCallable, Category = "nMatrix")
   void SetElement(int column, int row, float value);

   UFUNCTION(BlueprintCallable, Category = "nMatrix")
   void SetColumn(int column, UNVector *values);

   UFUNCTION(BlueprintCallable, Category = "nMatrix")
   void SetRow(int row, UNVector *values);



   // -------------------------------------------------------------------------------------------------
protected:
   void Init(TArray<UNVector *> matrix);
};

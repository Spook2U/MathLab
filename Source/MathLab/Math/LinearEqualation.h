// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NMatrix.h"
#include "LinearEqualation.generated.h"

/*
   A special formated Matrix with n rows and n+1 columns.
   Used to calculate Linear Equalations.
*/
USTRUCT(BlueprintType)
struct FLinearEqualation 
{
   GENERATED_BODY()

   FNMatrix CoefficientMatrix_solved;

   bool isSolved = false;

public:      
   /* LinearEqualation components. */
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|nMatrix")
   FNMatrix CoefficientMatrix;

   /* Default constructor (no initialization). */
   FORCEINLINE FLinearEqualation();

   /* Constructor to setup a coefficient matrix for a linear equalation. 
      The Matrix needs to be in the format: n+1 columns, n rows. */
   FORCEINLINE FLinearEqualation(const FNMatrix inMatrix);

   /* Checks if the coeffcient matrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck() const;
   /* Checks if the given nMatrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck(FNMatrix inMatrix) const;

   void Solve();

   FString ToString() const;
};

FORCEINLINE FLinearEqualation::FLinearEqualation() {}
FORCEINLINE FLinearEqualation::FLinearEqualation(const FNMatrix inMatrix)
{
   if(ValidCheck(inMatrix)) { CoefficientMatrix = inMatrix; }
}



FORCEINLINE bool FLinearEqualation::ValidCheck(FNMatrix inMatrix) const
{
   bool valid = true;

   if(inMatrix.ColumnNum() != (inMatrix.RowNum() + 1))
   {
      valid = false;
      MLD_ERR("Invalid input nMatrix for Linear Equalation. Expected format: n+1 columns, n rows; nMatrix has format: %d columns, %d rows", inMatrix.ColumnNum(), inMatrix.RowNum());
   }

   return valid;
}

FORCEINLINE bool FLinearEqualation::ValidCheck() const
{
   ValidCheck(CoefficientMatrix);
}

FORCEINLINE void FLinearEqualation::Solve()
{
   isSolved = true;

   float pivot;
   float rowPivot;

   for(int pivotIndex = 0, pMax = CoefficientMatrix.RowNum(); pivotIndex < pMax; pivotIndex++)
   {
      pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);
      MLD_LOG("Pivot: %f", pivot);
      
      //Spezialfall Pivot = 0
      if(pivot == 0)
      {
         //pivot Zeile mit einer Zeile weiter tiefer tausche, dann forfahren
      }

      CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
      
      for(int row = 0, rMax = CoefficientMatrix.RowNum(); row < rMax; row++)
      {
         if(row == pivotIndex) continue;

         rowPivot = CoefficientMatrix.GetElement(pivotIndex, row);
         MLD_LOG("Row Pivot: %f", rowPivot);

         if(rowPivot == 0) continue;

         CoefficientMatrix.SetRow(row, CoefficientMatrix.GetRow(row) + CoefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
         MLD_LOG("%s", *ToString());
      }
   }

}


FORCEINLINE FString FLinearEqualation::ToString() const
{
   FString s = CoefficientMatrix.ToString();

   if(isSolved)
   {
      s += ": ";
   }   

   return s;
}

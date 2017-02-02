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

public:      
   /* LinearEqualation components. */
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|nMatrix")
   FNMatrix CoefficientMatrix;

private: 
   bool isSolved = false;

   // Help variables to solve the linear equalation
   float pivot;
   int   pivotIndex;
   float rowPivot;
   int   rowPivotIndex;

public:      
   /* Default constructor (no initialization). */
   FORCEINLINE FLinearEqualation();

   /* Constructor to setup a coefficient matrix for a linear equalation. 
      The Matrix needs to be in the format: n+1 columns, n rows. */
   FORCEINLINE FLinearEqualation(const FNMatrix inMatrix);

   /* Checks if the coeffcient matrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck() const;
   /* Checks if the given nMatrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck(FNMatrix inMatrix) const;

   /* Solves this linear equalation. */
   void Solve();

   /* Get a textual representation of this linear equalation. */
   FString ToString() const;

private:
   bool CheckColumnZeroFromTo(int from, int to);
   bool CheckRowZeroFromTo(int from, int to);

   void SetPivot();
   bool LastPivot();
   bool SwitchRow();
   bool CheckColumnZero();
   void PivotToOne();
   bool MakeRowPivotToZero();
   bool CheckCoefficentZero();
   bool CheckRowZero();
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



/*
   foreach pivot
   {
      bChangePivot = true

      if(pivot == 0)
      {
         bChangePivot = false
      
         if(LetzterPivot())
         {
            beenden - Ergebnis: keine Lösung
         }

         if(ReiheNormierenMitIndexAufbau())
         {
            continue - Mitgleichem Index
         }

         if(ganze spalte 0)
         {
            continue - mit neuem Index
         }
      }
      else if(pivot == 1)
      {
         bChangePivot = false
      }
      
      if(bChangePivot)
      {
         Pivot 1 machen => pivotRow / pivot
      }
      
      foreach row != pivotIndex
      {
         if(rowVerarbeitrn())    //    if(rowPivot != 0) // rowPivot = element[pivotColumn][row]
         {   
            rowPivot_0_machen()     //   row + - rowPivot * pivotRow
            
            if(alle koeffizienten leer: zB 0 0 0 4) 
            {
               beenden - Ergebnis: keine Lösung
            }

            if(Reihe komplett leer: 0 0 0 0)
            {
               beenden - Ergebnis: keine eindeutige Lösung
            }
         }
      }
   }
*/

#define FINISH_NO_SOLUTON        pivotIndex = maxRows; /*ergebnis definieren*/ break
#define FINISH_ENDLESS_SOLUTIONS pivotIndex = maxRows; /*ergebnis definieren*/ break
#define REPEAT_WITH_SAME_INDEX   pivotIndex--; continue
#define SKIP                     continue

FORCEINLINE void FLinearEqualation::Solve()
{
   int maxRows = CoefficientMatrix.RowNum();

   isSolved = true;

   for(pivotIndex = 0; pivotIndex < maxRows; pivotIndex++)
   {
      SetPivot(); 
      
      if(pivot == 0)
      {
         if(LastPivot())       { FINISH_NO_SOLUTON; }
         if(SwitchRow())       { REPEAT_WITH_SAME_INDEX; }
         if(CheckColumnZero()) { SKIP; }
         
         //This Code should never be reached
         MLD_WAR("Not checked case of Pivot = 0 occured");
      }
      else if(pivot != 1)
      {
         PivotToOne();
      }
   
      for(rowPivotIndex = 0; rowPivotIndex < maxRows; rowPivotIndex++)
      {
         if(rowPivotIndex == pivotIndex) continue;
         
         if(MakeRowPivotToZero())
         {
            if(CheckCoefficentZero()) { FINISH_NO_SOLUTON; }
            if(CheckRowZero())        { FINISH_ENDLESS_SOLUTIONS; }
         }
      }
   }


   
   //bool pivotNotZero = true;
   //bool rowIsZero;
   //
   //for(int pivotIndex = 0, pMax = CoefficientMatrix.RowNum(); pivotIndex < pMax; pivotIndex++)
   //{
   //   pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);
   //   MLD_LOG("Pivot: %f", pivot);
   //   
   //   // Special case if pivot = 0;Row needs ot be switcehd, so pivot isn't 0
   //   if(pivot == 0)
   //   {
   //      int rowIndex = pivotIndex+1;
   //      bool found = false;
   //      
   //      pivotNotZero = false;
   //      
   //      // Search other rows under pivotRow for row with not 0 in pivotIndex column
   //      for(int rMax = CoefficientMatrix.RowNum(); rowIndex < rMax; rowIndex++)
   //      {
   //         if(CoefficientMatrix.GetElement(pivotIndex, rowIndex) != 0)
   //         {
   //            found = true;
   //            break;
   //         }
   //      }
   //
   //      // If found row, than switch pivot row with found row
   //      if(found)
   //      {
   //         FNVector pivotRow = CoefficientMatrix.GetRow(pivotIndex);
   //         CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(rowIndex));
   //         CoefficientMatrix.SetRow(rowIndex, pivotRow);
   //         MLD_LOG("switch rows: %s", *ToString());
   //         
   //         pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);
   //         MLD_LOG("New Pivot: %f", pivot);
   //
   //         pivotNotZero = true;
   //      }
   //
   //   }
   //
   //   // Make pivot element to 1
   //   if(pivotNotZero)
   //   {
   //      if(pivot != 1)
   //      { 
   //         CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
   //      }
   //   }
   //   else
   //   {
   //      //
   //      continue;
   //   }
   //   
   //   // Make all elements in same column as pivot 0
   //   for(int row = 0, rMax = CoefficientMatrix.RowNum(); row < rMax; row++)
   //   {
   //      if(row == pivotIndex) continue;
   //
   //      rowPivot = CoefficientMatrix.GetElement(pivotIndex, row);
   //      MLD_LOG("Row Pivot: %f", rowPivot);
   //
   //      if(rowPivot == 0) continue;
   //
   //      // Make element in colum from pivot 0
   //      CoefficientMatrix.SetRow(row, CoefficientMatrix.GetRow(row) + CoefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
   //      MLD_LOG("%s", *ToString());
   //
   //      // Check if one row has all elements on 0, except the last result row (e.g. 0 0 0 4)
   //      rowIsZero = true;
   //      for(int column = 0, cMax = CoefficientMatrix.ColumnNum()-1; column < cMax; column++)
   //      {
   //         if(CoefficientMatrix.GetElement(column, row) != 0)
   //         {
   //            rowIsZero = false;
   //         }
   //      }
   //      if(rowIsZero) break;
   //   }
   //   if(rowIsZero) break;
   //}
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



FORCEINLINE bool FLinearEqualation::CheckColumnZeroFromTo(int from, int to)
{
   bool isZero = true;

   if(from < 0)                             { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for from = %d", from); }
   else if(to > CoefficientMatrix.RowNum()) { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for to = %d", to); }
   else
   {
      for(int row = from; row < to; row++)
      {
         if(CoefficientMatrix.GetElement(pivotIndex, row) != 0)
         {
            isZero = false;
            break;
         }
      }
   }
   return isZero;
}

FORCEINLINE bool FLinearEqualation::CheckRowZeroFromTo(int from, int to)
{
   bool isZero = true;
   if(from < 0)                                { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for from = %d", from); }
   else if(to > CoefficientMatrix.ColumnNum()) { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for to = %d", to); }
   else
   {
      for(int column = from; column < to; column++)
      {
         if(CoefficientMatrix.GetElement(column, rowPivotIndex) != 0)
         {
            isZero = false;
            break;
         }
      }
   }
   return isZero;

}

FORCEINLINE void FLinearEqualation::SetPivot()
{
   pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);
}

FORCEINLINE bool FLinearEqualation::LastPivot()
{
   return pivotIndex == (CoefficientMatrix.RowNum() - 1);
}

FORCEINLINE bool FLinearEqualation::SwitchRow()
{
   int rowIndex = pivotIndex + 1;
   bool canSwitch = false;

   // Search other rows under pivotRow for row with not 0 in pivotIndex column
   for(int rMax = CoefficientMatrix.RowNum(); rowIndex < rMax; rowIndex++)
   {
      if(CoefficientMatrix.GetElement(pivotIndex, rowIndex) != 0)
      {
         canSwitch = true;
         break;
      }
   }

   // Switch rows
   if(canSwitch)
   {
      FNVector pivotRow = CoefficientMatrix.GetRow(pivotIndex);
      CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(rowIndex));
      CoefficientMatrix.SetRow(rowIndex, pivotRow);
   }

   return canSwitch;
}

FORCEINLINE bool FLinearEqualation::CheckColumnZero()
{
   return CheckColumnZeroFromTo(0, CoefficientMatrix.ColumnNum());
}

FORCEINLINE void FLinearEqualation::PivotToOne()
{
   if(pivot) CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
}

FORCEINLINE bool FLinearEqualation::MakeRowPivotToZero()
{
   bool didChange = false;

   rowPivot = CoefficientMatrix.GetElement(pivotIndex, rowPivotIndex);
   if(rowPivot != 0)
   {
      CoefficientMatrix.SetRow(rowPivotIndex, CoefficientMatrix.GetRow(rowPivotIndex) + CoefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
      didChange = true;
   }

   return didChange;
}

FORCEINLINE bool FLinearEqualation::CheckCoefficentZero()
{
   return CheckRowZeroFromTo(0, CoefficientMatrix.RowNum()-1);
}

FORCEINLINE bool FLinearEqualation::CheckRowZero()
{
   return CheckRowZeroFromTo(0, CoefficientMatrix.RowNum());
}

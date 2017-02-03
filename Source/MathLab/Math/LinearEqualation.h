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
   SolutionType solution = SolutionType::notSolved;

   bool debugging = true;

   // Help variables to solve the linear equalation
   float pivot;
   int   pivotIndex;
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

   SolutionType HasSolution() const;
   FNVector GetResults() const;

   /* Get a textual representation of this linear equalation. */
   FString ToString() const;
   /* Returns an Array of strings for each row of this linear equalation. */
   TArray<FString> ToStringRows() const;

private:
   bool IsSolved() const;

   //Help functions for Solve()
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
   void Solve_DebugLog(int row, FString notice);
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

#define FINISH_NO_SOLUTON        pivotIndex = maxRows; solution = SolutionType::no; break
#define FINISH_ENDLESS_SOLUTIONS pivotIndex = maxRows; solution = SolutionType::endless; break
#define REPEAT_WITH_SAME_INDEX   pivotIndex--; continue
#define SKIP                     continue

#define SOLVELOG(row, string)    Solve_DebugLog(row, string)

FORCEINLINE void FLinearEqualation::Solve()
{
   if(IsSolved()) return;

   int maxRows = CoefficientMatrix.RowNum();

   for(pivotIndex = 0; pivotIndex < maxRows; pivotIndex++)
   {
      SetPivot(); 
      
      if(pivot == 0)
      {
         if(LastPivot())       { FINISH_NO_SOLUTON; }
         if(SwitchRow())       { REPEAT_WITH_SAME_INDEX; }
         if(CheckColumnZero()) { SKIP; }
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
   
   if(!IsSolved()) { solution = SolutionType::one; }
   
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

FORCEINLINE SolutionType FLinearEqualation::HasSolution() const
{
   return solution;
}

FORCEINLINE FNVector FLinearEqualation::GetResults() const
{
   FNVector results;

   switch(solution)
   {
      case SolutionType::one:       results = CoefficientMatrix.GetColumn(CoefficientMatrix.ColumnNum()-1); break;
      case SolutionType::endless:   MLD_WAR("Noch nicht eingebaut, gucken was ich hier mache");             break;
      case SolutionType::notSolved: MLD_WAR("Linear Equalation not yet solved. Call Solve()."); 
      case SolutionType::no:        
      default:                      results = FNVector();
   }

   return results;
}



FORCEINLINE FString FLinearEqualation::ToString() const
{
   FString s = CoefficientMatrix.ToString();

   if(IsSolved())
   {
      s += ": ";
   }   

   return s;
}

FORCEINLINE TArray<FString> FLinearEqualation::ToStringRows() const
{
   return CoefficientMatrix.ToStringRows();
}

FORCEINLINE bool FLinearEqualation::IsSolved() const
{
   return solution != SolutionType::notSolved;
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
   
   SOLVELOG(pivotIndex, FString::Printf(TEXT("Pivot = %.1f"), pivot));
}

FORCEINLINE bool FLinearEqualation::LastPivot()
{
   bool isLast = pivotIndex == (CoefficientMatrix.RowNum() - 1);
   if(isLast)
   {
      SOLVELOG(pivotIndex, "Last Pivot = 0");
      MLD_LOG("End - No Solution");
      MLD_LOG("");
   }
   return isLast;
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
      SOLVELOG(pivotIndex, FString::Printf(TEXT("Switch (%d) with (%d)"), pivotIndex+1, rowIndex+1));
      FNVector pivotRow = CoefficientMatrix.GetRow(pivotIndex);
      CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(rowIndex));
      CoefficientMatrix.SetRow(rowIndex, pivotRow);
   }
   else
   {
      SOLVELOG(pivotIndex, "Skip changing Pivot, cause Pivot = 0");
   }

   return canSwitch;
}

FORCEINLINE bool FLinearEqualation::CheckColumnZero()
{
   bool isZero = CheckColumnZeroFromTo(0, CoefficientMatrix.RowNum());
   if(isZero) 
   {
      SOLVELOG(pivotIndex, FString::Printf(TEXT("Column %d is empty"), pivotIndex));
      MLD_LOG("Skip");
      MLD_LOG("");
   }
   return isZero;
}

FORCEINLINE void FLinearEqualation::PivotToOne()
{
   if(pivot) SOLVELOG(pivotIndex, FString::Printf(TEXT(": %.1f"), pivot));
   if(pivot) CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
}

FORCEINLINE bool FLinearEqualation::MakeRowPivotToZero()
{
   bool didChange = false;
   float rowPivot = CoefficientMatrix.GetElement(pivotIndex, rowPivotIndex);

   if(rowPivot != 0)
   {
      SOLVELOG(rowPivotIndex, FString::Printf(TEXT("+ %.1f*(%d)"), rowPivot*(-1), pivotIndex+1));
      CoefficientMatrix.SetRow(rowPivotIndex, CoefficientMatrix.GetRow(rowPivotIndex) + CoefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
      didChange = true;
   }
   else
   {
      SOLVELOG(rowPivotIndex, "Skip cause 0");
   }

   return didChange;
}

FORCEINLINE bool FLinearEqualation::CheckCoefficentZero()
{
   bool isZero = CheckRowZeroFromTo(0, CoefficientMatrix.ColumnNum()-1);
   if(isZero)
   {
      SOLVELOG(rowPivotIndex, "All coefficient 0");
      MLD_LOG("End - No Solution");
      MLD_LOG("");
   }
   return isZero;
}

FORCEINLINE bool FLinearEqualation::CheckRowZero()
{
   bool isZero = CheckRowZeroFromTo(0, CoefficientMatrix.ColumnNum());
   if(isZero)
   {
      SOLVELOG(pivotIndex, "Row is 0");
      MLD_LOG("End - Endless Solutions");
      MLD_LOG("");
   }
   return isZero;
}

FORCEINLINE void FLinearEqualation::Solve_DebugLog(int row, FString notice)
{
   if(debugging) 
   {
      FString output = "";
      TArray<FString> matrixStrings = CoefficientMatrix.ToStringRows();

      for(int i = 0; i < CoefficientMatrix.RowNum(); i++)
      {
         output = FString::Printf(TEXT("(%d) %s"), i+1, *matrixStrings[i]);
         if(i == row)
         {
            output += " | " + notice;
         }
         MLD_LOG("%s", *output);
      }
      MLD_LOG("");
   }
}

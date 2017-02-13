
#include "MathLab.h"
#include "LinearSystem.h"



FLSSolution::FLSSolution(){}
FLSSolution::FLSSolution(LSSolutionType inType, FNVector inSolution)
{
   type = inType;
   solution = inSolution;
}

// -------------------------------------------------------------------------------------------------

FLinearSystem::FLinearSystem() {}
FLinearSystem::FLinearSystem(const FNMatrix inMatrix)
{
   // Switch columns and rows, so the input is more readable, like a linear system on paper.
   CoefficientMatrix.Init(inMatrix.RowNum(), inMatrix.ColumnNum());
   for(int i = 0; i < inMatrix.ColumnNum(); i++)
   {
      CoefficientMatrix.SetRow(i, inMatrix.GetColumn(i));
   }
}

// -------------------------------------------------------------------------------------------------

#define SOLVELOG(row, string)    Solve_DebugLog(row, string)

FLSSolution FLinearSystem::GetSolution()
{
   if(IsSolved()) return solution;

   // Solving the linear system
   for(pivotIndex = 0; (pivotIndex < CoefficientMatrix.ColumnNum()-1) && (pivotIndex < CoefficientMatrix.RowNum()); pivotIndex++)
   {
      SetPivot(); 

      if(pivot == 0)
      {
         //if(LastPivot())       { break; }
         if(SwitchRow())       { pivotIndex--; continue; }
         if(CheckColumnZero()) { continue; }
      }
      else if(pivot != 1)
      {
         PivotToOne();
      }

      for(rowPivotIndex = 0; rowPivotIndex < CoefficientMatrix.RowNum(); rowPivotIndex++)
      {
         if(rowPivotIndex == pivotIndex) continue;

         MakeRowPivotToZero();
      }
   }

   // Interpretation of the solution
   if(CheckRowsZero())            { solution = FLSSolution(LSSolutionType::endless); }
   else if(CheckCoefficentZero()) { solution = FLSSolution(LSSolutionType::no);      }
   else
   {
      if(UniqueSolutionPossible())
      {
         solution = FLSSolution(LSSolutionType::one, CoefficientMatrix.GetColumn(CoefficientMatrix.ColumnNum()-1));
      }
      else
      {
         solution = FLSSolution(LSSolutionType::parameter, FNVector({CoefficientMatrix.GetElement(CoefficientMatrix.RowNum(), CoefficientMatrix.RowNum()-1)}));
      }
   }

   SolveLog(0, "", false);

   return solution;
}

// -------------------------------------------------------------------------------------------------

FString FLinearSystem::ToString() const
{
   FString s = "";

   s += "{";   

   for(int i = 0, j = CoefficientMatrix.ToStringRows().Num(); i < j; i++)
   {
      s += "{" + CoefficientMatrix.ToStringRows()[i] + "}";
      if(i < (j - 1))
      {
         s += ", ";
      }
   }
   s += "}";   

   if(IsSolved())
   {
      s += ": ";
   }   

   return s;
}

bool FLinearSystem::IsSolved() const{
   return solution.type != LSSolutionType::notSolved;
}

// -------------------------------------------------------------------------------------------------

bool FLinearSystem::CheckColumnZeroFromTo(int from, int to)
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

bool FLinearSystem::CheckRowZeroFromTo(int row, int from, int to)
{
   bool isZero = true;

   if(from < 0)                                { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for from = %d", from); }
   else if(to > CoefficientMatrix.ColumnNum()) { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for to = %d", to); }
   else
   {
      for(int column = from; column < to; column++)
      {
         if(CoefficientMatrix.GetElement(column, row) != 0)
         {
            isZero = false;
            break;
         }
      }
   }
   return isZero;

}

void FLinearSystem::SetPivot()
{
   pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);

   SolveLog(pivotIndex, FString::Printf(TEXT("Pivot = %.1f"), pivot));
}

bool FLinearSystem::LastPivot()
{
   bool isLast = pivotIndex == (CoefficientMatrix.RowNum() - 1);
   if(isLast)
   {
      SolveLog(pivotIndex, "Last Pivot = 0 -> No Solution");
   }
   return isLast;
}

bool FLinearSystem::SwitchRow()
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
      SolveLog(pivotIndex, FString::Printf(TEXT("Switch (%d) with (%d)"), pivotIndex+1, rowIndex+1));
      FNVector pivotRow = CoefficientMatrix.GetRow(pivotIndex);
      CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(rowIndex));
      CoefficientMatrix.SetRow(rowIndex, pivotRow);
   }
   else
   {
      SolveLog(pivotIndex, "Skip changing Pivot, cause Pivot = 0");
   }

   return canSwitch;
}

bool FLinearSystem::CheckColumnZero()
{
   bool isZero = CheckColumnZeroFromTo(0, CoefficientMatrix.RowNum());
   if(isZero) 
   {
      SolveLog(pivotIndex, FString::Printf(TEXT("Column %d is empty"), pivotIndex));
      MLD_LOG("Skip");
      MLD_LOG("");
   }
   return isZero;
}

void FLinearSystem::PivotToOne()
{
   if(pivot) SolveLog(pivotIndex, FString::Printf(TEXT(": %.1f"), pivot));
   if(pivot) CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
}

void FLinearSystem::MakeRowPivotToZero()
{
   float rowPivot = CoefficientMatrix.GetElement(pivotIndex, rowPivotIndex);

   if(rowPivot != 0)
   {
      SolveLog(rowPivotIndex, FString::Printf(TEXT("+ %.1f*(%d)"), rowPivot*(-1), pivotIndex+1));
      CoefficientMatrix.SetRow(rowPivotIndex, CoefficientMatrix.GetRow(rowPivotIndex) + CoefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
   }
   else
   {
      SolveLog(rowPivotIndex, "Skip cause 0");
   }
}

bool FLinearSystem::UniqueSolutionPossible()
{
   return CoefficientMatrix.RowNum() >= (CoefficientMatrix.ColumnNum() - 1);
}

bool FLinearSystem::CheckCoefficentZero()
{
   bool isZero = false;
   for(int row = 0; row < CoefficientMatrix.RowNum(); row++)
   {
      if(CheckRowZeroFromTo(row, 0, CoefficientMatrix.ColumnNum()-1))
      {
         isZero = true;
         break;
      }
   }

   return isZero;
}

bool FLinearSystem::CheckRowsZero()
{
   bool isZero = false;

   for(int row = 0; row < CoefficientMatrix.RowNum(); row++)
   {
      if(CheckRowZeroFromTo(row, 0, CoefficientMatrix.ColumnNum()))
      {
         isZero = true;
         break;
      }
   }

   return isZero;
}

void FLinearSystem::SolveLog(int row, FString notice, bool comment)
{
   if(debugging) 
   {
      FString output = "";
      TArray<FString> matrixStrings = CoefficientMatrix.ToStringRows();

      for(int i = 0; i < CoefficientMatrix.RowNum(); i++)
      {
         output = FString::Printf(TEXT("(%d) %s"), i+1, *matrixStrings[i]);
         if(comment) if(i == row) { output += " | " + notice; }
         MLD_LOG("%s", *output);
      }
      MLD_LOG("");
   }
}

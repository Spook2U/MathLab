
#include "MathLab.h"
#include "LinearSystem.h"



FLSSolution::FLSSolution(){}
FLSSolution::FLSSolution(LSSolutionType inType, FNVector inSolution) : type(inType), solution(inSolution) {}

// -------------------------------------------------------------------------------------------------

FLinearSystem::FLinearSystem() {}
FLinearSystem::FLinearSystem(const FNMatrix inMatrix)
{
   // Switch columns and rows, so the input is more readable, like a linear system on paper.
   coefficientMatrix.Init(inMatrix.RowNum(), inMatrix.ColumnNum());
   for(int i = 0; i < inMatrix.ColumnNum(); i++)
   {
      coefficientMatrix.SetRow(i, inMatrix.GetColumn(i));
   }
}

// -------------------------------------------------------------------------------------------------

#define SOLVELOG(row, string)    Solve_DebugLog(row, string)

FLSSolution FLinearSystem::GetSolution()
{
   if(IsSolved()) return solution;

   // Solving the linear system
   for(pivotIndex = 0; (pivotIndex < coefficientMatrix.ColumnNum()-1) && (pivotIndex < coefficientMatrix.RowNum()); pivotIndex++)
   {
      SetPivot(); 

      if(pivot == 0)
      {
         if(SwitchRow())       { pivotIndex--; continue; }
         if(CheckColumnZero()) { continue; }
      }
      else if(pivot != 1)
      {
         PivotToOne();
      }

      for(rowPivotIndex = 0; rowPivotIndex < coefficientMatrix.RowNum(); rowPivotIndex++)
      {
         if(rowPivotIndex == pivotIndex) continue;

         MakeRowPivotToZero();
      }
   }

   // Interpretation of the solution
   if(IsSolveable())
   {
      if(CountNonZeroRows() >= NumberOfVariables())
      {
         solution = FLSSolution(LSSolutionType::one, coefficientMatrix.GetColumn(coefficientMatrix.ColumnNum()-1));
      }
      else
      {
         solution = FLSSolution(LSSolutionType::endless, FNVector({coefficientMatrix.GetRow(CountNonZeroRows()-1)}));
      }
   }
   else
   {
      solution = FLSSolution(LSSolutionType::no);
   }

    SolveLog(0, "", false);

   return solution;
}

int FLinearSystem::NumberOfVariables()
{
   return coefficientMatrix.ColumnNum() - 1;
}

// -------------------------------------------------------------------------------------------------

FString FLinearSystem::ToString() const
{
   FString s = "";

   s += "{";   

   for(int i = 0, j = coefficientMatrix.ToStringRows().Num(); i < j; i++)
   {
      s += "{" + coefficientMatrix.ToStringRows()[i] + "}";
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
   else if(to > coefficientMatrix.RowNum()) { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for to = %d", to); }
   else
   {
      for(int row = from; row < to; row++)
      {
         if(coefficientMatrix.GetElement(pivotIndex, row) != 0)
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
   else if(to > coefficientMatrix.ColumnNum()) { MLD_ERR("CheckColumnZeroFromTo(int from, int to) invalid value for to = %d", to); }
   else
   {
      for(int column = from; column < to; column++)
      {
         if(coefficientMatrix.GetElement(column, row) != 0)
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
   pivot = coefficientMatrix.GetElement(pivotIndex, pivotIndex);

   SolveLog(pivotIndex, FString::Printf(TEXT("Pivot = %.1f"), pivot));
}

bool FLinearSystem::LastPivot()
{
   bool isLast = pivotIndex == (coefficientMatrix.RowNum() - 1);
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
   for(int rMax = coefficientMatrix.RowNum(); rowIndex < rMax; rowIndex++)
   {
      if(coefficientMatrix.GetElement(pivotIndex, rowIndex) != 0)
      {
         canSwitch = true;
         break;
      }
   }

   // Switch rows
   if(canSwitch)
   {
      SolveLog(pivotIndex, FString::Printf(TEXT("Switch (%d) with (%d)"), pivotIndex+1, rowIndex+1));
      FNVector pivotRow = coefficientMatrix.GetRow(pivotIndex);
      coefficientMatrix.SetRow(pivotIndex, coefficientMatrix.GetRow(rowIndex));
      coefficientMatrix.SetRow(rowIndex, pivotRow);
   }
   else
   {
      SolveLog(pivotIndex, "Skip changing Pivot, cause Pivot = 0");
   }

   return canSwitch;
}

bool FLinearSystem::CheckColumnZero()
{
   bool isZero = CheckColumnZeroFromTo(0, coefficientMatrix.RowNum());
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
   if(pivot) coefficientMatrix.SetRow(pivotIndex, coefficientMatrix.GetRow(pivotIndex) / pivot);
}

void FLinearSystem::MakeRowPivotToZero()
{
   float rowPivot = coefficientMatrix.GetElement(pivotIndex, rowPivotIndex);

   if(rowPivot != 0)
   {
      SolveLog(rowPivotIndex, FString::Printf(TEXT("+ %.1f*(%d)"), rowPivot*(-1), pivotIndex+1));
      coefficientMatrix.SetRow(rowPivotIndex, coefficientMatrix.GetRow(rowPivotIndex) + coefficientMatrix.GetRow(pivotIndex) * ((-1) * rowPivot));
   }
   else
   {
      SolveLog(rowPivotIndex, "Skip cause 0");
   }
}

bool FLinearSystem::IsSolveable()
{
   bool solveable = true;
   for(int row = 0; row < coefficientMatrix.RowNum(); row++)
   {
      // Check if all coeffiecients are 0
      if(CheckRowZeroFromTo(row, 0, coefficientMatrix.ColumnNum()-1))
      {
         // Check if last column is not 0
         if(coefficientMatrix.GetElement(coefficientMatrix.ColumnNum()-1, row) != 0)
         {
            solveable = false;
            break;
         }
      }
   }

   return solveable;
}

int FLinearSystem::CountNonZeroRows()
{
   int zeroRows = 0;

   for(int row = 0; row < coefficientMatrix.RowNum(); row++)
   {
      if(CheckRowZeroFromTo(row, 0, coefficientMatrix.ColumnNum()))
      {
         zeroRows++;
      }
   }

   return coefficientMatrix.RowNum() - zeroRows;
}

void FLinearSystem::SolveLog(int row, FString notice, bool comment)
{
   if(debugging) 
   {
      FString output = "";
      TArray<FString> matrixStrings = coefficientMatrix.ToStringRows();

      for(int i = 0; i < coefficientMatrix.RowNum(); i++)
      {
         output = FString::Printf(TEXT("(%d) %s"), i+1, *matrixStrings[i]);
         if(comment) if(i == row) { output += " | " + notice; }
         MLD_LOG("%s", *output);
      }
      MLD_LOG("");
   }
}


/**/
#include "MathLab.h"
#include "LinearEqualation.h"


FLinearEqualation::FLinearEqualation() {}
FLinearEqualation::FLinearEqualation(const FNMatrix inMatrix)
{
   if(ValidCheck(inMatrix)) { CoefficientMatrix = inMatrix; }
}



bool FLinearEqualation::ValidCheck(FNMatrix inMatrix) const
{
   bool valid = true;

   if(inMatrix.ColumnNum() != (inMatrix.RowNum() + 1))
   {
      valid = false;
      MLD_ERR("Invalid input nMatrix for Linear Equalation. Expected format: n+1 columns, n rows; nMatrix has format: %d columns, %d rows", inMatrix.ColumnNum(), inMatrix.RowNum());
   }

   return valid;
}

bool FLinearEqualation::ValidCheck() const
{
   return ValidCheck(CoefficientMatrix);
}



#define FINISH_NO_SOLUTON        pivotIndex = maxRows; solution = LinEqSolution::no; break
#define FINISH_ENDLESS_SOLUTIONS pivotIndex = maxRows; solution = LinEqSolution::endless; break
#define REPEAT_WITH_SAME_INDEX   pivotIndex--; continue
#define SKIP                     continue

#define SOLVELOG(row, string)    Solve_DebugLog(row, string)

void FLinearEqualation::Solve()
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

   if(!IsSolved()) { solution = LinEqSolution::one; }

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

LinEqSolution FLinearEqualation::HasSolution() const
{
   return solution;
}

FNVector FLinearEqualation::GetSolutionOne() 
{
   FNVector result;

   if(!IsSolved()) Solve();

   switch(solution)
   {
      case LinEqSolution::one:       result = CoefficientMatrix.GetColumn(CoefficientMatrix.ColumnNum()-1); break;
      case LinEqSolution::endless:   MLD_WAR("This linear equalation has more than one solution. Use GetSolutionEndless() instead."); break;
      case LinEqSolution::no:        MLD_WAR("This linear equalation has no solution. Use HasSolution() to check.")
      default:                      result = FNVector();
   }

   return result;
}

FNVector FLinearEqualation::GetSolutionEndless()
{
   FNVector results;

   if(!IsSolved()) Solve();

   switch(solution)
   {
      case LinEqSolution::one:       MLD_WAR("This linear equalation has only one solution. Use GetSolutionOne() instead."); break;
      case LinEqSolution::endless:   MLD_WAR("Noch nicht eigebaut"); break;
      case LinEqSolution::no:        MLD_WAR("This linear equalation has no solution. Use HasSolution() to check.")
      default:                      results = FNVector();
   }

   return results;
}




FString FLinearEqualation::ToString() const
{
   FString s = CoefficientMatrix.ToString();

   if(IsSolved())
   {
      s += ": ";
   }   

   return s;
}

TArray<FString> FLinearEqualation::ToStringRows() const
{
   return CoefficientMatrix.ToStringRows();
}

bool FLinearEqualation::IsSolved() const
{
   return solution != LinEqSolution::notSolved;
}


bool FLinearEqualation::CheckColumnZeroFromTo(int from, int to)
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

bool FLinearEqualation::CheckRowZeroFromTo(int from, int to)
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

void FLinearEqualation::SetPivot()
{
   pivot = CoefficientMatrix.GetElement(pivotIndex, pivotIndex);

   SOLVELOG(pivotIndex, FString::Printf(TEXT("Pivot = %.1f"), pivot));
}

bool FLinearEqualation::LastPivot()
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

bool FLinearEqualation::SwitchRow()
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

bool FLinearEqualation::CheckColumnZero()
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

void FLinearEqualation::PivotToOne()
{
   if(pivot) SOLVELOG(pivotIndex, FString::Printf(TEXT(": %.1f"), pivot));
   if(pivot) CoefficientMatrix.SetRow(pivotIndex, CoefficientMatrix.GetRow(pivotIndex) / pivot);
}

bool FLinearEqualation::MakeRowPivotToZero()
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

bool FLinearEqualation::CheckCoefficentZero()
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

bool FLinearEqualation::CheckRowZero()
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

void FLinearEqualation::Solve_DebugLog(int row, FString notice)
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
/**/
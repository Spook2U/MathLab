
#include "MathLab.h"
#include "NMatrix.h"

FNMatrix::FNMatrix() {}
FNMatrix::FNMatrix(const TArray<FNVector> inNMatrix) : Matrix(inNMatrix) {}
FNMatrix::FNMatrix(int column, int row)
{
   for(int i = 0; i < column; i++)
   {
      Matrix.Add(FNVector(row));
   }
}

FNMatrix& FNMatrix::operator=(const FNMatrix& Other)
{
   this->Matrix = Other.Matrix;

   return *this;
}



bool FNMatrix::ValidCheck() const
{
   bool validSize = true;
   int sizeOfFirstColumn = 0;

   if(ColumnNum() < 0) 
   {
      validSize = false;
      MLD_ERR("Cannot execute; Empty nMatrix.");
   }
   else
   {
      sizeOfFirstColumn = Matrix[0].Size();
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         if(Matrix[i].Size() != sizeOfFirstColumn)
         {
            validSize = false;
            MLD_ERR("nMatrix wrong format. Column %d has wrong size; Expected length: %d; %s", i, sizeOfFirstColumn, *ToString());
            break;
         }
      }
   }

   return validSize;
}

int FNMatrix::ColumnNum() const
{
   return Matrix.Num();
}

int FNMatrix::RowNum() const
{
   if(!ValidCheck()) return 0;
   return Matrix[0].Size();
}



float FNMatrix::GetElement(int column, int row) const
{
   float element = 0.f;

   if((column < 0) || (column >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: column = %d", column); }
   else if((row < 0) || (row >= RowNum()))     { MLD_ERR("nMatrix out of Bounds: row = %d",    row); }
   else
   {
      element = Matrix[column].Get(row);
   }

   return element;
}

FNVector FNMatrix::GetColumn(int column) const
{
   FNVector wantedColumn = FNVector();

   if(!ValidCheck()) return wantedColumn;

   if((column < 0) || (column >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: column = %d", column); }
   else
   {
      wantedColumn = Matrix[column];
   }
   return wantedColumn;
}

FNVector FNMatrix::GetRow(int row) const
{
   FNVector wantedRow = FNVector();

   if(!ValidCheck()) return wantedRow;

   if((row < 0) || (row >= RowNum())) { MLD_ERR("nMatrix out of Bounds: row = %d", row); }
   else
   {
      for(FNVector column : Matrix)
      {
         wantedRow.Add(column.Get(row));
      }
   }
   return wantedRow;
}



void FNMatrix::Set(FNMatrix inNMatrix)
{
   Matrix = inNMatrix.Matrix;
}

void FNMatrix::Set(TArray<FNVector> inNMatrix)
{
   Set(FNMatrix(inNMatrix));
}

void FNMatrix::SetElement(int column, int row, float value) 
{
   if((column < 0) || (column >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: column = %d", column); }
   else if((row < 0) || (row >= RowNum()))     { MLD_ERR("nMatrix out of Bounds: row = %d",    row); }
   else
   {
      Matrix[column].Set(row, value);
   }
}

void FNMatrix::SetColumn(int index, FNVector column) 
{
   if((index < 0) || (index >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else if(column.Size() != RowNum())        { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else
   {
      Matrix[index] = column;      
   }
}

void FNMatrix::SetColumn(int index, TArray<float> column) 
{
   SetColumn(index, FNVector(column));
}

void FNMatrix::SetRow(int index, FNVector row) 
{
   if((index < 0) || (index >= RowNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else if(row.Size() != ColumnNum())     { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", ColumnNum()); }
   else
   {
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         Matrix[i].Set(index, row.Get(i));
      }
   }
}

void FNMatrix::SetRow(int index, TArray<float> row) 
{
   SetRow(index, FNVector(row));
}



void FNMatrix::AddColumn(TArray<float> column)
{
   if(column.Num() != RowNum()) { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else                         { Matrix.Add(column); }
}

void FNMatrix::AddColumn(FNVector column)
{
   AddColumn(column.Coordinates);
}

void FNMatrix::AddRow(TArray<float> row)
{
   if(row.Num() != ColumnNum()) { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", ColumnNum()); }
   else
   {
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         Matrix[i].Add(row[i]);
      }
   }
}

void FNMatrix::AddRow(FNVector row)
{
   AddRow(row.Coordinates);
}



void FNMatrix::RemoveColumnAt(int index)
{
   if((index < 0) || (index > ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else { Matrix.RemoveAt(index); }
}

void FNMatrix::RemoveRowAt(int index)
{
   if((index < 0) || (index > RowNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else 
   { 
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         Matrix[i].RemoveAt(index);
      }
   }
}




FString FNMatrix::ToString() const
{
   FString s = "";

   s += "{";
   for(int i = 0; i < Matrix.Num(); i++)
   {
      s += Matrix[i].ToString();
      if(i < (Matrix.Num() - 1))
      {
         s += ", ";
      }
   }
   s += "}";
   return s;
}

TArray<FString> FNMatrix::ToStringRows() const
{
   TArray<FString> rows;

   for(int i = 0; i < RowNum(); i++)
   {
      rows.Add(GetRow(i).ToString());
   }

   return rows;
}

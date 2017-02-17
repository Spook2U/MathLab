
#include "MathLab.h"
#include "NMatrix.h"



FNMatrix::FNMatrix() {}
FNMatrix::FNMatrix(const TArray<FNVector> inNMatrix) : matrix(inNMatrix) {}

FNMatrix& FNMatrix::operator=(const FNMatrix& Other)
{
   this->matrix = Other.matrix;

   return *this;
}

// -------------------------------------------------------------------------------------------------

void FNMatrix::Init(int column, int row)
{
   matrix.Empty();
   FNVector v = FNVector();
   v.Init(row);
   for(int i = 0; i < column; i++)
   {
      matrix.Add(v);
   }
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
      sizeOfFirstColumn = matrix[0].Size();
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         if(matrix[i].Size() != sizeOfFirstColumn)
         {
            validSize = false;
            MLD_ERR("nMatrix wrong format. Column %d has wrong size; Expected length: %d; %s", i, sizeOfFirstColumn, *ToString());
            break;
         }
      }
   }

   return validSize;
}

// -------------------------------------------------------------------------------------------------

int FNMatrix::ColumnNum() const
{
   return matrix.Num();
}

int FNMatrix::RowNum() const
{
   if(!ValidCheck()) return 0;
   return matrix[0].Size();
}

// -------------------------------------------------------------------------------------------------

float FNMatrix::GetElement(int column, int row) const
{
   float element = 0.f;

   if((column < 0) || (column >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: column = %d", column); }
   else if((row < 0) || (row >= RowNum()))     { MLD_ERR("nMatrix out of Bounds: row = %d",    row); }
   else
   {
      element = matrix[column].Get(row);
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
      wantedColumn = matrix[column];
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
      for(FNVector column : matrix)
      {
         wantedRow.Add(column.Get(row));
      }
   }
   return wantedRow;
}

// -------------------------------------------------------------------------------------------------

void FNMatrix::Set(FNMatrix inNMatrix)
{
   matrix = inNMatrix.matrix;
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
      matrix[column].Set(row, value);
   }
}

void FNMatrix::SetColumn(int index, FNVector column) 
{
   if((index < 0) || (index >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else if(column.Size() != RowNum())        { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else
   {
      matrix[index] = column;      
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
         matrix[i].Set(index, row.Get(i));
      }
   }
}

void FNMatrix::SetRow(int index, TArray<float> row) 
{
   SetRow(index, FNVector(row));
}

// -------------------------------------------------------------------------------------------------

void FNMatrix::AddColumn(TArray<float> column)
{
   if(column.Num() != RowNum()) { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else                         { matrix.Add(column); }
}

void FNMatrix::AddColumn(FNVector column)
{
   AddColumn(column.coordinates);
}

void FNMatrix::AddRow(TArray<float> row)
{
   if(row.Num() != ColumnNum()) { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", ColumnNum()); }
   else
   {
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         matrix[i].Add(row[i]);
      }
   }
}

void FNMatrix::AddRow(FNVector row)
{
   AddRow(row.coordinates);
}

// -------------------------------------------------------------------------------------------------

void FNMatrix::RemoveColumnAt(int index)
{
   if((index < 0) || (index > ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else { matrix.RemoveAt(index); }
}

void FNMatrix::RemoveRowAt(int index)
{
   if((index < 0) || (index > RowNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else 
   { 
      for(int i = 0, j = ColumnNum(); i < j; i++)
      {
         matrix[i].RemoveAt(index);
      }
   }
}

// -------------------------------------------------------------------------------------------------

FString FNMatrix::ToString() const
{
   FString s = "";

   s += "{";
   for(int i = 0; i < matrix.Num(); i++)
   {
      s += matrix[i].ToString();
      if(i < (matrix.Num() - 1))
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NVector.h"
#include "NMatrix.generated.h"

/*
   A matrix saving up data in an array of nVectors
   Needs multiple nVectors of same size
*/
USTRUCT(BlueprintType)
struct FNMatrix 
{
   GENERATED_BODY()

public:      
   /* nMatrix's components. */
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|nMatrix")
   TArray<FNVector> Matrix;

   /* Default constructor (no initialization). */
   FORCEINLINE FNMatrix();
   /* Constructor to make a nMatrix. */
   FORCEINLINE FNMatrix(const TArray<FNVector> inNMatrix);
   /* Constructor to make a nMatrix initialised with 0. */
   FORCEINLINE FNMatrix(int column, int row);

   /* Copy another nVector into this one. */
   FORCEINLINE FNMatrix& operator=(const FNMatrix& Other);

   /* Checks if all nVectors in Matrix have the same length. */
   bool ValidCheck() const;

   /* Returns the number of Columns in nMatrix. */
   int ColumnNum() const;
   /* Returns the number of Columns in nMatrix. */
   int RowNum() const;

   /* Returns the element at the given colum and row. */
   float GetElement(int column, int row) const;
   /* Returns a nVector containing the wanted column. */
   FNVector GetColumn(int column) const;
   /* Returns a nVector containing the wanted row. */
   FNVector GetRow(int row) const;

   /* Uses the given nMatrix to reinitialise the matrix. */
   void Set(FNMatrix inNMatrix);
   /* Uses the given nVector Array to reinitialise the matrix. */
   void Set(TArray<FNVector> inNMatrix);
   /* Sets the Element at the given column and row. */
   void SetElement(int column, int row, float value);
   /* Sets the elements of the column at index to the given nVector. */
   void SetColumn(int index, FNVector column);
   /* Sets the elements of the column at index to the given TArray. */
   void SetColumn(int index, TArray<float> column);
   /* Sets the elements of the row at index to the given nVector. */
   void SetRow(int index, FNVector row);
   /* Sets the elements of the row at index to the given TArray. */
   void SetRow(int index, TArray<float> row);

   /* Adds a column at the end of the array; Increases the number of rows by 1. */
   void AddColumn(TArray<float> column);
   /* Adds a column at the end of the array; Increases the number of rows by 1. */
   void AddColumn(FNVector column);
   /* Adds a row at the end of the array; Increases the number of columns by 1. */
   void AddRow(TArray<float> row);
   /* Adds a row at the end of the array; Increases the number of columns by 1. */
   void AddRow(FNVector row);

   /* Removes a column at the given index; Decreases number of rows by 1. */
   void RemoveColumnAt(int index);
   /* Removes a row at the given index; Decreases number of columns by 1. */
   void RemoveRowAt(int index);

   /* Get a textual representation of this nMatrix. */
   FString ToString() const;
   /* Returns an Array of strings for each row of this nMatrix. */
   TArray<FString> ToStringRows() const;
};

FORCEINLINE FNMatrix::FNMatrix() {}
FORCEINLINE FNMatrix::FNMatrix(const TArray<FNVector> inNMatrix) : Matrix(inNMatrix) {}
FORCEINLINE FNMatrix::FNMatrix(int column, int row)
{
   for(int i = 0; i < column; i++)
   {
      Matrix.Add(FNVector(row));
   }
}

FORCEINLINE FNMatrix& FNMatrix::operator=(const FNMatrix& Other)
{
   this->Matrix = Other.Matrix;

   return *this;
}



FORCEINLINE bool FNMatrix::ValidCheck() const
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

FORCEINLINE int FNMatrix::ColumnNum() const
{
   return Matrix.Num();
}

FORCEINLINE int FNMatrix::RowNum() const
{
   if(!ValidCheck()) return 0;
   return Matrix[0].Size();
}



FORCEINLINE float FNMatrix::GetElement(int column, int row) const
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

FORCEINLINE FNVector FNMatrix::GetColumn(int column) const
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

FORCEINLINE FNVector FNMatrix::GetRow(int row) const
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



FORCEINLINE void FNMatrix::Set(FNMatrix inNMatrix)
{
   Matrix = inNMatrix.Matrix;
}

FORCEINLINE void FNMatrix::Set(TArray<FNVector> inNMatrix)
{
   Set(FNMatrix(inNMatrix));
}

FORCEINLINE void FNMatrix::SetElement(int column, int row, float value) 
{
   if((column < 0) || (column >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: column = %d", column); }
   else if((row < 0) || (row >= RowNum()))     { MLD_ERR("nMatrix out of Bounds: row = %d",    row); }
   else
   {
      Matrix[column].Set(row, value);
   }
}

FORCEINLINE void FNMatrix::SetColumn(int index, FNVector column) 
{
   if((index < 0) || (index >= ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else if(column.Size() != RowNum())        { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else
   {
      Matrix[index] = column;      
   }
}

FORCEINLINE void FNMatrix::SetColumn(int index, TArray<float> column) 
{
   SetColumn(index, FNVector(column));
}

FORCEINLINE void FNMatrix::SetRow(int index, FNVector row) 
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

FORCEINLINE void FNMatrix::SetRow(int index, TArray<float> row) 
{
   SetRow(index, FNVector(row));
}



FORCEINLINE void FNMatrix::AddColumn(TArray<float> column)
{
   if(column.Num() != RowNum()) { MLD_ERR("nMatrix wrong format. Input nVector has wrong size; Expected size: %d", RowNum()); }
   else                         { Matrix.Add(column); }
}

FORCEINLINE void FNMatrix::AddColumn(FNVector column)
{
   AddColumn(column.Coordinates);
}

FORCEINLINE void FNMatrix::AddRow(TArray<float> row)
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

FORCEINLINE void FNMatrix::AddRow(FNVector row)
{
   AddRow(row.Coordinates);
}



FORCEINLINE void FNMatrix::RemoveColumnAt(int index)
{
   if((index < 0) || (index > ColumnNum())) { MLD_ERR("nMatrix out of Bounds: index = %d", index); }
   else { Matrix.RemoveAt(index); }
}

FORCEINLINE void FNMatrix::RemoveRowAt(int index)
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




FORCEINLINE FString FNMatrix::ToString() const
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

FORCEINLINE TArray<FString> FNMatrix::ToStringRows() const
{
   TArray<FString> rows;

   for(int i = 0; i < RowNum(); i++)
   {
      rows.Add(GetRow(i).ToString());
   }

   return rows;
}

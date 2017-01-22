// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "NMatrix.h"

UNMatrix::UNMatrix()
{
   Matrix.Empty();
}

UNMatrix *UNMatrix::MakeNMatrix(TArray<UNVector *> matrix)
{
   UNMatrix *v = NewObject<UNMatrix>();

   MLD_PTR_CHECK(v); if(!v) return nullptr;

   v->Init(matrix);

   return v;
}

FString UNMatrix::ToString()
{
   FString s = "";

   s += "nMatrix: {";
   for(int i = 0; i < Matrix.Num(); i++)
   {
      s += Matrix[i]->ToString();
      if(i < (Matrix.Num() - 1))
      {
         s += ", ";
      }
   }
   s += "}";
   return s;
}

// Getter ------------------------------------------------------------------------------------------

float UNMatrix::GetElement(int column, int row)
{
   float element = 0.f;

   MLD_PTR_CHECK(Matrix[column]); if(!Matrix[column]) return 0.f;

   //Catch invalid Input
   if(column >= Matrix.Num())                        { MLD_ERR("Matrix out of Bounds: column = %d", column); }
   else if(row >= Matrix[column]->Coordinates.Num()) { MLD_ERR("Matrix out of Bounds: row = %d", row); }
   else
   {
      element = Matrix[column]->Coordinates[row];
   }
   return element;
}

UNVector *UNMatrix::GetColumn(int column)
{
   UNVector *v = nullptr;

   //Catch invalid Input
   if((column < 0) || (column >= Matrix.Num())) { MLD_ERR("Matrix out of Bounds: column = %d", column); }
   else
   {
      v = NewObject<UNVector>();
      MLD_PTR_CHECK(v); if(!v) return nullptr;
      MLD_PTR_CHECK(Matrix[column]); if(!Matrix[column]) return nullptr;

      v = Matrix[column];
   }
   return v;
}

UNVector *UNMatrix::GetRow(int row)
{
   UNVector *v = nullptr;

   //To check First index is enough to proove => Init() check if all nVector have same length
   MLD_PTR_CHECK(Matrix[0]); if(!Matrix[0]) return nullptr;

   //Catch invalid Input
   if(row >= Matrix[0]->Coordinates.Num()) { MLD_ERR("Matrix out of Bounds: row = %d", row); }
   else
   {
      v = NewObject<UNVector>();
      MLD_PTR_CHECK(v); if(!v) return nullptr;

      for(UNVector *nv : Matrix)
      {
         MLD_PTR_CHECK(nv); if(!nv) return nullptr;

         v->Coordinates.Add(nv->Coordinates[row]);
      }
   }
   return v;
}

// Setter ------------------------------------------------------------------------------------------

void UNMatrix::SetElement(int column, int row, float value)
{
   MLD_PTR_CHECK(Matrix[column]); if(!Matrix[column]) return ;

   //Catch invalid Input
   if(column >= Matrix.Num())                        { MLD_ERR("Matrix out of Bounds: column = %d", column); }
   else if(row >= Matrix[column]->Coordinates.Num()) { MLD_ERR("Matrix out of Bounds: row = %d", row); }
   else
   {
      Matrix[column]->Coordinates[row] = value;
   }
}

void UNMatrix::SetColumn(int column, UNVector * values)
{
}

void UNMatrix::SetRow(int row, UNVector * values)
{
}


// Protected ----------------------------------------------------------------------------------------

void UNMatrix::Init(TArray<UNVector *> matrix)
{
   this->Matrix = matrix;
}



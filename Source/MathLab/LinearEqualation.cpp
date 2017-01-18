// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "LinearEqualation.h"

ULinearEqualation::ULinearEqualation()
{
   CoefficientMatrix.Empty();
   NumColumns = 0;
   NumRows = 0;
}



ULinearEqualation *ULinearEqualation::MakeLinearEqualation(TArray<UNVector *> coefficientMatrix)
{
   ULinearEqualation *le = NewObject<ULinearEqualation>();
   MLD_PTR_CHECK(le);
   if(le)
   {
      le->Init(coefficientMatrix);
   }
   return le;
}



UNVector *ULinearEqualation::Solve()
{
   float pivot = 0;
   float rowPivot = 0;

   MLD_LOG("NumRows: %d, NumColumn: %d", NumRows, NumColumns);

   for(int pivotIndex = 0; pivotIndex < NumRows; pivotIndex++)
   {
      pivot = GetElement(pivotIndex, pivotIndex);
      //SpezialFall pivot = 0
      
      // Zeiger an der Stelle &(GetRow(pivotIndex)->Coordinates[0])      
      for(int i = 0; i < GetRow(pivotIndex)->Coordinates.Num(); i++)
      {
         GetRow(pivotIndex)->Coordinates[0] /= pivot;
      }

      for(int row = 0; row < NumRows; row++)
      {
         //Not the Row where the pivot is
         if(row != pivotIndex)
         {
            rowPivot = GetElement(pivotIndex, row);
            
            //set rowPivot to 0, skip if already 0
            if(rowPivot != 0)
            {
               for(float f : GetRow(row)->Coordinates)
               {
                  for(int column = 0; column < NumColumns; column++)
                  {
                     f = GetElement(column, row) + GetElement(column, pivotIndex) * (-1)*rowPivot;
                  }
               }
            }
         }
      }
   }

   return GetColumn(NumColumns-1);
}



FString ULinearEqualation::ToString()
{
   FString s = "";

   s += "{";
   for(int i = 0; i < CoefficientMatrix.Num(); i++)
   {
      s += CoefficientMatrix[i]->ToString();
      if(i < (CoefficientMatrix.Num() - 1))
      {
         s += ", ";
      }
   }
   s += "}";
   return s;
}

// -------------------------------------------------------------------------------------------------

float ULinearEqualation::GetElement(int column, int row)
{
   float element = 0.0f;

   //Catch invalid Input
   if(column >= CoefficientMatrix.Num())                        { MLD_ERR("Array out of Bounds: column = %d", column); }
   else if(row >= CoefficientMatrix[column]->Coordinates.Num()) { MLD_ERR("Array out of Bounds: row = %d", row); }
   else
   {
      MLD_PTR_CHECK(CoefficientMatrix[column]);
      if(CoefficientMatrix[column])
      {
         element = CoefficientMatrix[column]->Coordinates[row];
      }
   }
   return element;
}

UNVector *ULinearEqualation::GetColumn(int column)
{
   UNVector *v = NewObject<UNVector>();

   //Catch invalid Input
   if(column >= CoefficientMatrix.Num()) { MLD_ERR("Array out of Bounds: column = %d", column); }
   else
   {
      MLD_PTR_CHECK(CoefficientMatrix[column]);
      if(CoefficientMatrix[column])
      {
         MLD_PTR_CHECK(v);
         if(v)
         {
            v = CoefficientMatrix[column];
         }
      }
   }
   return v;
}

UNVector *ULinearEqualation::GetRow(int row)
{
   UNVector *v = NewObject<UNVector>();

   //Catch invalid Input
   //To check First index is enough to proove => Init() check if all nVector have same length
   MLD_PTR_CHECK(CoefficientMatrix[0]);
   if(CoefficientMatrix[0])
   {
      if(row >= CoefficientMatrix[0]->Coordinates.Num()) { MLD_ERR("Array out of Bounds: row = %d", row); }
      else
      {
         for(UNVector *nv : CoefficientMatrix)
         {
            MLD_PTR_CHECK(nv);
            if(nv)
            {
               MLD_PTR_CHECK(v);
               if(v)
               {
                  v->Coordinates.Add(nv->Coordinates[row]);
               }
            }
         }
      }
   }
   return v;
}

void ULinearEqualation::Init(TArray<UNVector*> coefficientMatrix)
{
   bool validNVectorSize = true;
   bool validNVectorNumber = true;
   int tempLength = 0;
   
   UNVector *errorVector = nullptr;

   //All input nVectors need to have the same size
   MLD_PTR_CHECK(coefficientMatrix[0]);
   if(coefficientMatrix[0])
   {
      tempLength = coefficientMatrix[0]->Coordinates.Num();
   }

   for(UNVector *v : coefficientMatrix)
   {
      MLD_PTR_CHECK(v);
      if(v)
      {
         if(v->Coordinates.Num() != tempLength)
         {
            errorVector = v;
            validNVectorSize = false;
            break;
         }
      }
   }

   //Matrix needs to have the format: n rows and n+1 columns
   if(validNVectorSize)
   {
      if(coefficientMatrix.Num() != (tempLength + 1))
      {
         validNVectorNumber = false;
      }
   }

   if(     !validNVectorSize)   { MLD_ERR("Invalid Input: nVector %s has wrong size: %d, should be: %d", *errorVector->ToString(), errorVector->Coordinates.Num(), tempLength); }
   else if(!validNVectorNumber) { MLD_ERR("Invalid Input: m nVector with the size m-1 needed. Number of nVectors: %d, nVector Size: %d", coefficientMatrix.Num(), tempLength); }
   else
   {
      this->CoefficientMatrix = coefficientMatrix;
      NumRows = tempLength;
      NumColumns = NumRows+1;
   }
}

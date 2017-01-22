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

   MLD_PTR_CHECK(le); if(!le) return nullptr;
   le->Init(coefficientMatrix);

   return le;
}



UNVector *ULinearEqualation::Solve()
{
   //float pivot = 0;
   //float rowPivot = 0;

   //for(int pivotIndex = 0; pivotIndex < NumRows; pivotIndex++)
   //{
   //   pivot = GetElement(pivotIndex, pivotIndex);
   //   //SpezialFall pivot = 0
   //   
   //   // Zeiger an der Stelle &(GetRow(pivotIndex)->Coordinates[0])      
   //   for(int i = 0; i < GetRow(pivotIndex)->Coordinates.Num(); i++)
   //   {
   //      GetRow(pivotIndex)->Coordinates[i] /= pivot;
   //   }

   //   for(int row = 0; row < NumRows; row++)
   //   {
   //      //Not the Row where the pivot is
   //      if(row != pivotIndex)
   //      {
   //         rowPivot = GetElement(pivotIndex, row);
   //         
   //         //set rowPivot to 0, skip if already 0
   //         if(rowPivot != 0)
   //         {
   //            for(int i = 0; i < GetRow(row)->Coordinates.Num(); i++)
   //            {
   //               for(int column = 0; column < NumColumns; column++)
   //               {
   //                  GetRow(row)->Coordinates[i] = GetElement(column, row) + GetElement(column, pivotIndex) * (-1)*rowPivot;
   //               }
   //            }
   //         }
   //      }
   //   }
   //}

   UNVector *test = NewObject<UNVector>();

   MLD_PTR_CHECK(testMatrix); if(!testMatrix) return test;

   MLD_LOG("matrix %s", *testMatrix->ToString()); 
   testMatrix->SetElement(1, 3, 42.f);
   MLD_LOG("matrix %s", *testMatrix->ToString()); 

   //MLD_LOG("row %s",    *testMatrix->GetRow(0)->ToString());
   //MLD_LOG("column %s", *testMatrix->GetColumn(0)->ToString());
   //MLD_LOG("element %f", testMatrix->GetElement(0,0));

   return test;
}



FString ULinearEqualation::ToString()
{
   FString s = "";

   s += "nMatrix: {";
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

void ULinearEqualation::Init(TArray<UNVector*> coefficientMatrix)
{
   bool validNVectorSize = true;
   bool validNVectorNumber = true;
   int tempLength = 0;
   
   UNVector *errorVector = nullptr;

   //All input nVectors need to have the same size
   MLD_PTR_CHECK(coefficientMatrix[0]); if(!coefficientMatrix[0]) return;

   tempLength = coefficientMatrix[0]->Coordinates.Num();

   for(UNVector *v : coefficientMatrix)
   {
      MLD_PTR_CHECK(v); if(!v) return;

      if(v->Coordinates.Num() != tempLength)
      {
         errorVector = v;
         validNVectorSize = false;
         break;
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
      
      testMatrix = UNMatrix::MakeNMatrix(coefficientMatrix);
   }
}

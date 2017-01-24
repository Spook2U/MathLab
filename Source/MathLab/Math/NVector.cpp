// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "NVector.h"

UNVector::UNVector()
{
   Coordinates.Empty();
}

FString UNVector::ToString()
{
   FString s = "";
   
   s += "nVector: {";
   for(int i = 0, max = Coordinates.Num(); i < max; i++)
   {
      s += FString::SanitizeFloat(Coordinates[i]);
      if(i < (max - 1))
      {
         s += ", ";
      }
   }
   s += "}";
   return s;
}

// Protected ----------------------------------------------------------------------------------------

void UNVector::Init(TArray<float> coordinates)
{
   this->Coordinates = coordinates;
}




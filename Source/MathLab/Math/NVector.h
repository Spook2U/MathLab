// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MathLab.h"
#include "NVector.generated.h"

/*
   A vector in N-D space composed of an array of components with floating point precision.
*/
USTRUCT(BlueprintType)
struct FNVector 
{
   GENERATED_BODY()

public:      
   /* nVector's components. */
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|nVector")
   TArray<float> Coordinates;

   /* Default constructor (no initialization). */
   FORCEINLINE FNVector();
   /* Constructor to make a nvector with n-Dimensions. */
   FORCEINLINE FNVector(TArray<float> inCoordinates);
   /* Constructor to make a zero nVector with n-Dimensions. */
   FORCEINLINE FNVector(int dimenion);

   bool ValidCheck(FNVector inNVector) const;


   /* Copy another nVector into this one. */
   FORCEINLINE FNVector& operator=(const FNVector& Other);
   /* Gets the result of component-wise addition of this and another vector. */
   FORCEINLINE FNVector operator+(const FNVector& V) const;
   /* Gets the result of component-wise subtraction of this by another vector. */
   FORCEINLINE FNVector operator-(const FNVector& V) const;
   /* Gets the result of scaling the vector (multiplying each component by a value). */
   FORCEINLINE FNVector operator*(float Scale) const;
   /* Gets the result of dividing each component of the vector by a value. */
   FNVector operator/(float Scale) const;
   ///* Gets the result of component-wise multiplication of this vector by another. */
   //FORCEINLINE FNVector operator*(const FNVector& V) const;
   ///* Gets the result of component-wise division of this vector by another. */
   //FORCEINLINE FNVector operator/(const FNVector& V) const;
   /* Check against another vector for equality. */
   bool operator==(const FNVector& V) const;
   /* Check against another vector for inequality. */
   bool operator!=(const FNVector& V) const;
   /* Adds another vector to this. Uses component-wise addition. */
   FORCEINLINE FNVector operator+=(const FNVector& V);
   /* Subtracts another vector from this. Uses component-wise subtraction. */
   FORCEINLINE FNVector operator-=(const FNVector& V);
   /* Scales the vector. */
   FORCEINLINE FNVector operator*=(float Scale);
   /* Divides the vector by a number. */
   FNVector operator/=(float V);
   ///* Multiplies the vector with another vector, using component-wise multiplication. */
   //FNVector operator*=(const FNVector& V);
   ///* Divides the vector by another vector, using component-wise division. */
   //FNVector operator/=(const FNVector& V);



   /* Returns the coordinate at the given index. */
   float Get(int index) const;

   /* Sets the coordinate at the given index. */
   void Set(int index, float value);
   /* Uses the given nVector to reinitialise the coordinates. */
   void Set(FNVector inNVector);
   /* Uses the given array to reinitialise the coordinates. */
   void Set(TArray<float> coordinates);

   /* Adds a coordinate at the end of the array; Increases the dimension by 1. */
   void Add(float value);
   /* Appends the elements of the given Array; Increases the dimension by te size of the given Array. */
   void Append(TArray<float> coordinates);

   /* Removes a coordinate at the given index; Decreases the dimension by 1. */
   void RemoveAt(int index);
   
   /* returns the number of coordinates in nVector. */
   int Size() const;

   /* Get a textual representation of this nVector. */
   FString ToString() const;
};



FORCEINLINE FNVector::FNVector() {}
FORCEINLINE FNVector::FNVector(TArray<float> inCoordinates) : Coordinates(inCoordinates) {}
FORCEINLINE FNVector::FNVector(int dimension)
{
   for(int i = 0; i < dimension; i++)
   {
      Coordinates.Add(0);
   }
}

FORCEINLINE bool FNVector::ValidCheck(FNVector inNVector) const
{
   bool valid = true;

   if(Size() != inNVector.Size())
   {
      valid = false;
      MLD_ERR("Invalid input nVector; Size %d expected, Invalid size: %d", Size(), inNVector.Size());
   }

   return valid;
}

FORCEINLINE FNVector& FNVector::operator=(const FNVector& Other)
{
   this->Coordinates = Other.Coordinates;
   
   return *this;
}


FORCEINLINE FNVector FNVector::operator+(const FNVector& V) const
{
   FNVector result = FNVector(Coordinates);
   
   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] += V.Get(i);
      }
   }

   return result;
}

FORCEINLINE FNVector FNVector::operator-(const FNVector& V) const
{
   FNVector result = FNVector(Coordinates);

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] -= V.Get(i);
      }
   }

   return result;
}

FORCEINLINE FNVector FNVector::operator*(float Scale) const
{
   FNVector result = FNVector(Coordinates);

   for(int i = 0; i < Size(); i++)
   {
      result.Coordinates[i] *= Scale;
   }

   return result;

}

FORCEINLINE FNVector FNVector::operator/(float Scale) const
{
   FNVector result = FNVector(Coordinates);

   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         result.Coordinates[i] /= Scale;
      }
   }
   else
   {
      MLD_ERR("Division through 0 not allowed!");
   }

   return result;
}

//FORCEINLINE FNVector FNVector::operator*(const FNVector& V) const {}
//FORCEINLINE FNVector FNVector::operator/(const FNVector& V) const {}

FORCEINLINE bool FNVector::operator==(const FNVector& V) const
{
   bool equal = true; 

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         if(Get(i) != V.Get(i))
         {
            equal = false;
            break;
         }
      }
   }

   return equal;
}

FORCEINLINE bool FNVector::operator!=(const FNVector& V) const
{
   bool notEqual = false; 

   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         if(Get(i) != V.Get(i))
         {
            notEqual = true;
            break;
         }
      }
   }

   return notEqual;
}

FORCEINLINE FNVector FNVector::operator+=(const FNVector& V)
{
   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] += V.Get(i);
      }
   }

   return *this;
}

FORCEINLINE FNVector FNVector::operator-=(const FNVector& V)
{
   if(ValidCheck(V))
   { 
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] -= V.Get(i);
      }
   }


   return *this;
}

FORCEINLINE FNVector FNVector::operator*=(float Scale)
{
   for(int i = 0; i < Size(); i++)
   {
      Coordinates[i] *= Scale;
   }

   return *this;
}

FORCEINLINE FNVector FNVector::operator/=(float Scale)
{
   if(Scale != 0)
   {
      for(int i = 0; i < Size(); i++)
      {
         Coordinates[i] /= Scale;
      }
   }
   else
   {
      MLD_ERR("Division through 0 not allowed!");
   }

   return *this;
}

//FORCEINLINE FNVector FNVector::operator*=(const FNVector& V) {}
//FORCEINLINE FNVector FNVector::operator/=(const FNVector& V) {}



FORCEINLINE float FNVector::Get(int index) const 
{
   float coordinate = 0;
   
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { coordinate = Coordinates[index]; }

   return coordinate;
}

FORCEINLINE void FNVector::Set(int index, float value) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { Coordinates[index] = value; }
}

FORCEINLINE void FNVector::Set(FNVector inNVector)
{
   Coordinates = inNVector.Coordinates;
}

FORCEINLINE void FNVector::Set(TArray<float> coordinates) 
{
   Set(FNVector(coordinates));
}

FORCEINLINE void FNVector::Add(float value) 
{
   Coordinates.Add(value);
}

FORCEINLINE void FNVector::Append(TArray<float> coordinates)
{
   for(float f : coordinates)
   {
      Add(f);
   }
}

FORCEINLINE void FNVector::RemoveAt(int index) 
{
   if((index < 0) || (index > Size())) { MLD_ERR("nVector out of Bounds: index = %d", index); }
   else { Coordinates.RemoveAt(index); }
}



FORCEINLINE int FNVector::Size() const
{
   return Coordinates.Num();
}

FORCEINLINE FString FNVector::ToString() const
{
   FString s = "";

   s += "{";
   for(int i = 0, max = Coordinates.Num(); i < max; i++)
   {
      if(Coordinates[i] < 10 && Coordinates[i] > -10)
      {
         s += " ";
      }
      if(Coordinates[i] >= 0) 
      { 
         s += " "; 
      }
      s += FString::SanitizeFloat(Coordinates[i]);
      if(i < (max - 1))      
      { 
         s += ", "; 
      }
   }
   s += "}";
   
   return s;
}

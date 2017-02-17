// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
   UPROPERTY(BlueprintReadWrite, Category = "nVector")
   TArray<float> coordinates;

   /* Default constructor (no initialization). */
   FNVector();
   /* Constructor to make a nvector with n-Dimensions. */
   FNVector(TArray<float> inCoordinates);
   
   /* Initialises the nVector with 0.*/
   void Init(int dimension);

   /* Checks if the inNVector size is compatible with itself. */
   bool ValidCheck(FNVector inNVector) const;

   /* returns the number of coordinates in nVector. */
   int Size() const;

   /* Copy another nVector into this one. */
   FNVector& operator=(const FNVector& Other);
   /* Gets the result of component-wise addition of this and another vector. */
   FNVector operator+(const FNVector& V) const;
   /* Gets the result of component-wise subtraction of this by another vector. */
   FNVector operator-(const FNVector& V) const;
   /* Gets the result of scaling the vector (multiplying each component by a value). */
   FNVector operator*(float Scale) const;
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
   FNVector operator+=(const FNVector& V);
   /* Subtracts another vector from this. Uses component-wise subtraction. */
   FNVector operator-=(const FNVector& V);
   /* Scales the vector. */
   FNVector operator*=(float Scale);
   /* Divides the vector by a number. */
   FNVector operator/=(float V);
   ///* Multiplies the vector with another vector, using component-wise multiplication. */
   //FNVector operator*=(const FNVector& V);
   ///* Divides the vector by another vector, using component-wise division. */
   //FNVector operator/=(const FNVector& V);

   /* Converts the nVector to FVector using the first 3 coordinates. */
   FVector GetFVector();

   /* Returns the coordinate at the given index. */
   float Get(int index) const;
   /* Sets the coordinate at the given index. */
   void Set(int index, float value);
   /* Uses the given nVector to reinitialise the coordinates. */
   void Set(FNVector inNVector);
   /* Uses the given array to reinitialise the coordinates. */
   void Set(TArray<float> inCoordinates);
   /* Adds a coordinate at the end of the array; Increases the dimension by 1. */
   void Add(float value);
   /* Appends the elements of the given Array; Increases the dimension by te size of the given Array. */
   void Append(TArray<float> inCoordinates);
   /* Removes a coordinate at the given index; Decreases the dimension by 1. */
   void RemoveAt(int index);
   
   /* Get a textual representation of this nVector. */
   FString ToString() const;
};
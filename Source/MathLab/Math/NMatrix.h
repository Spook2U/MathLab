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
   FNMatrix();
   /* Constructor to make a nMatrix. */
   FNMatrix(const TArray<FNVector> inNMatrix);

   /* Copy another nVector into this one. */
   FNMatrix& operator=(const FNMatrix& Other);

   /* Initialises the nMatrix with 0.*/
   void Init(int column, int row);

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
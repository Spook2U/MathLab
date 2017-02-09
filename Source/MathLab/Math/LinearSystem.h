// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NMatrix.h"
#include "LinearSystem.generated.h"



/* Defines the different solution types a Linear System can have. */
UENUM(BlueprintType)
enum class LSSolutionType : uint8
{
   one       UMETA(DisplayName = "Defined Solution"),   
   no        UMETA(DisplayName = "Not solvable"),   
   endless   UMETA(DisplayName = "Endless Solutions"),
   notSolved UMETA(DisplayName = "Not Solved")
};



/* Contains the solution for a System of Linear Equalations. */
USTRUCT(BlueprintType)
struct FLSSolution
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|Linear System")
   LSSolutionType type = LSSolutionType::notSolved;
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|Linear System")
   TArray<FNVector> solution;
   
   /* Default constructor (no initialization). */
   FLSSolution();
   /* Constructor to initialise the Linear System Solution structure. */
   FLSSolution(LSSolutionType inType, TArray<FNVector> inSolution = TArray<FNVector>());
};



/*
   Holds a System of Linear Equalations in a (n)x(n+1) nMatrix
*/
USTRUCT(BlueprintType)
struct FLinearSystem 
{
   GENERATED_BODY()

public:      
   /* LinearEqualation components. */
   UPROPERTY(BlueprintReadWrite, Category = "Math Lab|nMatrix")
   FNMatrix CoefficientMatrix;

private: 
   FLSSolution solution = FLSSolution(LSSolutionType::notSolved);

   bool  debugging = true;
   float pivot;
   int   pivotIndex;
   int   rowPivotIndex;

public:      
   /* Default constructor (no initialization). */
   FLinearSystem();

   /* Constructor to setup a coefficient matrix for a linear equalation. 
      The Matrix needs to be in the format: n+1 columns, n rows. */
   FLinearSystem(const FNMatrix inMatrix);

   /* Checks if the coeffcient matrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck() const;
   /* Checks if the given nMatrix has the correct format: n+1 columns, n rows. */
   bool ValidCheck(FNMatrix inMatrix) const;

   /* Solves this linear equalation. */
   FLSSolution Solve();

   /* Get a textual representation of this linear equalation. */
   FString ToString() const;
   /* Returns an Array of strings for each row of this linear equalation. */
   TArray<FString> ToStringRows() const;

private:
   bool IsSolved() const;

   //Help functions for Solve()
   bool CheckColumnZeroFromTo(int from, int to);
   bool CheckRowZeroFromTo(int from, int to);
   void SetPivot();
   bool LastPivot();
   bool SwitchRow();
   bool CheckColumnZero();
   void PivotToOne();
   bool MakeRowPivotToZero();
   bool CheckCoefficentZero();
   bool CheckRowZero();
   void Solve_DebugLog(int row, FString notice);
};

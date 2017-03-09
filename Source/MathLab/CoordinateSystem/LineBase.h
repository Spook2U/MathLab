// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "LineBase.generated.h"


// Line Mode -----------------------------------------------------------------------------------------------------------------------------------------


/* Defines the mode of the line. */
UENUM(BlueprintType)
enum class LineMode : uint8
{
   line    UMETA(DisplayName = "Line"),
   segment UMETA(DisplayName = "Segment"),
   vector  UMETA(DisplayName = "Vector")
};


// Math Line Structure -------------------------------------------------------------------------------------------------------------------------------


/* Defines a mathematical line. */
USTRUCT(BlueprintType)
struct FMathLine
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "line")
   FVector position;
   UPROPERTY(BlueprintReadWrite, Category = "line")
   FVector direction;

   /* Default constructor (no initialization). */
   FMathLine();
   /* Constructor to set the line with position and direction. */
   FMathLine(FVector inPosition, FVector inDirection);

   /* Copy another line into this one. */
   FMathLine& operator=(const FMathLine& Other);
   /* Check against another line for equality. */
   bool operator==(const FMathLine& Other) const;
   /* Check against another line for inequality. */
   bool operator!=(const FMathLine& Other) const;

   /* Get a textual representation of this Circle. */
   FString ToString();
   FString ToStringShort();
};

// Line Class ----------------------------------------------------------------------------------------------------------------------------------------

UCLASS()
class MATHLAB_API ALineBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   ALineBase();
   
   virtual void BeginPlay() override;

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FMathLine line;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   LineMode mode;

private:
   UStaticMeshComponent *lineMesh;
   UStaticMeshComponent *arrowheadMesh;

// Functions -----------------------------------------------------------------------------------------------------------------------------------------

public:
// Line Setup ----------------------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathLine inLine, LineMode inMode, FName inName = "");

   /* Saves the StaticMeshReferences in the source file to work on. */
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   /* Called, when the objects need to update the position or other values. */
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   virtual void Update() override;

   /* Sets up the components to display the line in the selected mode. */
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   void BuildLine();

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

   /* Sets the MathLine struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   ALineBase *SetLine(FMathLine inLine);

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

   UFUNCTION(Blueprintcallable, Category = "math lab|line")
   virtual FString ToString() override;

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

   virtual void CreateCVector(LaserColors inColor) override;

};

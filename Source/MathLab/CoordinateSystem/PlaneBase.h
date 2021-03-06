// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PlaneBase.generated.h"

struct FMathPoint;


// Plane Mode ----------------------------------------------------------------------------------------------------------------------------------------


UENUM(BlueprintType)
enum class PlaneMode : uint8
{
   plane   UMETA(DisplayName = "Plane")
};


// Math Plane Structure ------------------------------------------------------------------------------------------------------------------------------


/* Defines a mathematical plane. */
USTRUCT(BlueprintType)
struct FMathPlane
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "plane")
   FVector position;
   UPROPERTY(BlueprintReadWrite, Category = "plane")
   FVector direction1;
   UPROPERTY(BlueprintReadWrite, Category = "plane")
   FVector direction2;

   FVector normal;
   float d;

   /* Default constructor (no initialization). */
   FMathPlane();
   /* Constructor to set the plane with parametic form position + direction 1 + direction 2. */
   FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2);
   /* Constructor to set the plane with normal form position and normal. */
   FMathPlane(FMathPoint inPoint, FVector inNormal);
   /* Constructor to set the plane with coordinate form with ax + by + cz = d. */
   FMathPlane(float a, float b, float c, float d);

   /* Copy another plane into this one. */
   FMathPlane& operator=(const FMathPlane& Other);
   /* Check against another plane for equality. */
   bool operator==(const FMathPlane& Other) const;
   /* Check against another plane for inequality. */
   bool operator!=(const FMathPlane& Other) const;

   /* Builds the normal Vector based on it's members. */
   void BuildNormal();
   /* Builds the value d based on it's members. */
   void BuildD();
   /* Sets up the MathPlane from the normalform*/
   void BuildFromNormalform(FMathPoint inPoint, FVector inNormal);

   /* Returns the unified normal Vector of the plane. */
   FVector GetNormal();
   /* Returns the value d of the koordinate form: ax + by + cz = d. */
   float GetD();

   /* Get a textual representation of this Plane. */
   FString ToString();
   FString ToStringShort();

private:
   bool isNormalSet;
   bool isDSet;

};


// Plane Class ---------------------------------------------------------------------------------------------------------------------------------------


UCLASS()
class MATHLAB_API APlaneBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APlaneBase();

   virtual void BeginPlay() override;

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FMathPlane plane;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   PlaneMode mode;

private:
   UStaticMeshComponent *planeMesh;

// Functions -----------------------------------------------------------------------------------------------------------------------------------------

public:
// Plane Setup ---------------------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPlane inPlane, PlaneMode inMode, FName inName = "");

   /* Saves the StaticMeshReferences in the source file to work on. */
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   /* Called, when the objects need to update the position or other values. */
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   virtual void Update() override;

   /* Sets up the components to display the plane in the selected mode. */
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   void BuildPlane();

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

   /* Sets the MathPlane struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   APlaneBase *SetPlane(FMathPlane inPlane);

   /* Returns the unified normal Vector of the plane. */
   UFUNCTION(BlueprintPure, Category = "math lab|plane")
   FVector GetNormal();

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

   UFUNCTION(Blueprintcallable, Category = "math lab|plane")
   virtual FString ToString() override;

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

   virtual void CreateCVector(LaserColors inColor) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "SphereBase.generated.h"


// Math Sphere Structure -----------------------------------------------------------------------------------------------------------------------------


/* Defines a mathematical sphere. */
USTRUCT(BlueprintType)
struct FMathSphere
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "sphere")
   FVector center;
   UPROPERTY(BlueprintReadWrite, Category = "sphere")
   float radius;

   /* Default constructor (no initialization). */
   FMathSphere();
   /* Constructor to set the coordinate of the point. */
   FMathSphere(FVector inCenter, float inRadius);

   /* Copy another sphere into this one. */
   FMathSphere& operator=(const FMathSphere& Other);
   /* Check against another sphere for equality. */
   bool operator==(const FMathSphere& Other) const;
   /* Check against another sphere for inequality. */
   bool operator!=(const FMathSphere& Other) const;

   /* Get a textual representation of this Plane. */
   FString ToString();
   FString ToStringShort();
};


// Sphere Class --------------------------------------------------------------------------------------------------------------------------------------

 
UCLASS()
class MATHLAB_API ASphereBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   ASphereBase();

   virtual void BeginPlay() override;

public:
// Member --------------------------------------------------------------------------------------------------------------------------------------------

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "sphere")
   FMathSphere sphere;

private:
   UStaticMeshComponent *sphereMesh;

// Functions -----------------------------------------------------------------------------------------------------------------------------------------

public:
// Sphere Setup --------------------------------------------------------------------------------------------------------------------------------------

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathSphere inSphere, FString inName = "");

   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

// Update Functions ----------------------------------------------------------------------------------------------------------------------------------

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   virtual void Update() override;

   //Sets up the Components to display the sphere with the specified radus
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   void BuildSphere();

// Setting Functions ---------------------------------------------------------------------------------------------------------------------------------

   /* Sets the MathSphere struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|sphere")
   ASphereBase *SetSphere(FMathSphere inSphere);

// Utility Functions----------------------------------------------------------------------------------------------------------------------------------

   UFUNCTION(Blueprintcallable, Category = "math lab|sphere")
   virtual FString ToString() override;

// Constructing Vector Functions ---------------------------------------------------------------------------------------------------------------------

   virtual void CreateCVector(LaserColors inColor) override;

};

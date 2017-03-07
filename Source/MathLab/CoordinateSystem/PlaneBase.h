// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PlaneBase.generated.h"

struct FMathPoint;



UENUM(BlueprintType)
enum class PlaneMode : uint8
{
   plane   UMETA(DisplayName = "Plane")
};



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
   /* Constructor to set the plane with position and 2 directions. */
   FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2);
   /* Constructor to set the plane with position and normal. */
   FMathPlane(FMathPoint inPoint, FVector inNormal);

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

// -------------------------------------------------------------------------------------------------

UCLASS()
class MATHLAB_API APlaneBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APlaneBase();

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   FMathPlane plane;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "plane")
   PlaneMode mode;

private:
   UStaticMeshComponent *planeMesh;



public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPlane inPlane, PlaneMode inMode, FString inName = "");

   /* Returns the unified normal Vector of the plane. */
   UFUNCTION(BlueprintPure, Category = "math lab|plane")
   FVector GetNormal();

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   virtual void Update() override;

   //Sets up the Components to display the plane in the selected mode
   UFUNCTION(BlueprintCallable, Category = "math lab|plane")
   void BuildPlane();

   UFUNCTION(Blueprintcallable, Category = "math lab|plane")
   virtual FString ToString() override;

protected:
   virtual void CreateCVector(LaserColors inColor) override;

	
	
};

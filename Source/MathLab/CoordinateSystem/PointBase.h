// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "PointBase.generated.h"



/* Defines a mathematical Point. */
USTRUCT(BlueprintType)
struct FMathPoint
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "point")
   FVector coordinate;

   /* Default constructor (no initialization). */
   FMathPoint();
   /* Constructor to set the coordinate of the point. */
   FMathPoint(FVector inCoordinate);

   /* Copy another point into this one. */
   FMathPoint& operator=(const FMathPoint& Other);
   /* Check against another point for equality. */
   bool operator==(const FMathPoint& Other) const;
   /* Check against another point for inequality. */
   bool operator!=(const FMathPoint& Other) const;

   /* Get a textual representation of this Point. */
   FString ToString();
   FString ToStringShort();
};

// -------------------------------------------------------------------------------------------------

UCLASS()
class MATHLAB_API APointBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   APointBase();

   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "point")
   FMathPoint point;

private:
   UStaticMeshComponent *pointMesh;
   


public:
   //Saves the StaticMeshReferences in the source file to work on
   UFUNCTION(BlueprintCallable, Category = "math lab|point")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathPoint inPoint, FString inName = "");

   //Called, when the objects need to update the position or other vales
   UFUNCTION(BlueprintCallable, Category = "math lab|point")
   virtual void Update() override;

   /* Sets the MathPoint struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|point")
   APointBase *SetPoint(FMathPoint inPoint);

   UFUNCTION(Blueprintcallable, Category = "math lab|point")
   virtual FString ToString() override;


   
protected:
   virtual void CreateCVector(LaserColors inColor) override;	

	
	
	
};

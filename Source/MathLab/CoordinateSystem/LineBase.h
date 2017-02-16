// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "LineBase.generated.h"



UENUM(BlueprintType)
enum class LineMode : uint8
{
   line    UMETA(DisplayName = "Line"),
   segment UMETA(DisplayName = "Segment"),
   vector  UMETA(DisplayName = "Vector")
};



/* Defines a mathematical line. */
USTRUCT(BlueprintType)
struct FMathLine
{
   GENERATED_BODY()

public:      
   UPROPERTY(BlueprintReadWrite, Category = "line")
   FVector Position;
   UPROPERTY(BlueprintReadWrite, Category = "line")
   FVector Direction;

   /* Default constructor (no initialization). */
   FMathLine();
   /* Constructor to set the line with position and direction. */
   FMathLine(FVector inPosition, FVector inDirection);
};



UCLASS()
class MATHLAB_API ALineBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   ALineBase();
   
   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FMathLine line;
   LineMode Mode;

private:
   UStaticMeshComponent *lineMesh;
   UStaticMeshComponent *arrowheadMesh;

// -------------------------------------------------------------------------------------------------

public:
   /**Saves the StaticMeshReferences in the source file to work on*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void InitLine(ACoordinateSystemBase *coordinateSystem, LaserColors color, FMathLine inLine, LineMode mode);

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Update")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Setup")
   void BuildLine();
   
protected:
   virtual void CreateVectorGuides(LaserColors color) override;
};

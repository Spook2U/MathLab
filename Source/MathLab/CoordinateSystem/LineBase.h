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

// -------------------------------------------------------------------------------------------------

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
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   LineMode mode;

private:
   UStaticMeshComponent *lineMesh;
   UStaticMeshComponent *arrowheadMesh;



public:
   /**Saves the StaticMeshReferences in the source file to work on*/
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   void SetComponents(TArray<UStaticMeshComponent *> components, UTextRenderComponent *inText);

   void Init(ACoordinateSystemBase *inCoordinateSystem, LaserColors inColor, FMathLine inLine, LineMode inMode, FString inName = "");

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   virtual void Update() override;

   /* Sets the MathLine struct. */
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   ALineBase *SetLine(FMathLine inLine);

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "math lab|line")
   void BuildCVector();

   UFUNCTION(Blueprintcallable, Category = "math lab|line")
   virtual FString ToString() override;

protected:
   virtual void CreateCVector(LaserColors inColor) override;
};

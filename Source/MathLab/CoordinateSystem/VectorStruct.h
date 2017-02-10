// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "VectorStruct.generated.h"



UENUM(BlueprintType)
enum class VectorStructMode : uint8
{
   point        UMETA(DisplayName = "Point"),
   segment      UMETA(DisplayName = "Segment"),
   vector       UMETA(DisplayName = "Vector"),
   vectorPoint  UMETA(DisplayName = "Vector to Point"),
   general      UMETA(DisplayName = "General")
};



UCLASS()
class MATHLAB_API AVectorStruct : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   AVectorStruct();

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   FVector A;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   FVector B;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vectorStruct")
   VectorStructMode Mode;

private:
   UStaticMeshComponent *PointA;
   UStaticMeshComponent *PointB;
   UStaticMeshComponent *Line;
   UStaticMeshComponent *Arrowhead;

// -------------------------------------------------------------------------------------------------

public:
   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct")
   void SetComponents(TArray<UStaticMeshComponent *> laserComponents);

   void SetValuesVectorStruct(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector a, FVector b, VectorStructMode mode = VectorStructMode::general);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibilityPointA(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibilityPointB(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibilityLine(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibilityArrowhead(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibilityForAll(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct|Visibility")
   void SetVisibility(bool showPointA, bool showPointB, bool showLine, bool showArrowhead);

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct")
   void BuildLine();

   /**Moves PointB to the defined coordinate*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|vectorStruct")
   void SetPointB();
};

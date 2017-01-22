// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "VectorStruct.generated.h"

UCLASS()
class MATHLAB_API AVectorStruct : public AGeometryBase
{
	GENERATED_BODY()
	
public:
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "vectorStruct")
   FVector A;
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "vectorStruct")
   FVector B;
   
private:
   UStaticMeshComponent *PointA;
   UStaticMeshComponent *PointB;
   UStaticMeshComponent *Line;
   UStaticMeshComponent *Arrowhead;

// -------------------------------------------------------------------------------------------------

public:
   UFUNCTION(BlueprintCallable, Category = "vectorStruct")
   void SetComponents(TArray<UStaticMeshComponent *> laserComponents);
   
   UFUNCTION(BlueprintCallable, Category = "vectorStruct")
   void SetVisibilityPointA(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "vectorStruct")
   void SetVisibilityPointB(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "vectorStruct")
   void SetVisibilityLine(bool visibility);

   UFUNCTION(BlueprintCallable, Category = "vectorStruct")
   void SetVisibilityArrowhead(bool visibility);

   void SetValuesVectorStruct(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector a, FVector b);

	
};

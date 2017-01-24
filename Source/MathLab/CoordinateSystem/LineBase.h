// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/GeometryBase.h"
#include "LineBase.generated.h"

UCLASS()
class MATHLAB_API ALineBase : public AGeometryBase
{
	GENERATED_BODY()

public:
   ALineBase();
   
   virtual void BeginPlay() override;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FVector Position;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   FVector Direction;
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "line")
   LineMode Mode;

private:
   UStaticMeshComponent *Line;
   UStaticMeshComponent *Arrowhead;

// -------------------------------------------------------------------------------------------------

public:
   /**Saves the StaticMeshReferences in the source file to work on*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|plane|Init")
   void SetComponents(TArray<UStaticMeshComponent *> components);

   void SetValuesLine(ACoordinateSystemBase *coordinateSystem, LaserColors color, FVector position, FVector direction, LineMode mode);

   /**Called, when the objects need to update the position or other vales*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Update")
   virtual void Update() override;

   /**Sets up the Components to display the line in the selected mode*/
   UFUNCTION(BlueprintCallable, Category = "coordinate System|line|Setup")
   void BuildLine();
   
protected:
   virtual void CreateVectorGuides(LaserColors color) override;
};

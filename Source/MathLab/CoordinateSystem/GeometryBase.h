// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoordinateSystemBase.h"
#include "GeometryBase.generated.h"

UCLASS()
class MATHLAB_API AGeometryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGeometryBase();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

public:

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   ACoordinateSystemBase *CoordinateSystem;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   bool IsGuide;

public:
   void SetValues(ACoordinateSystemBase *coordinateSystem, LaserColors color);

   UFUNCTION(BlueprintCallable, Category = "Geometry")
   void ShowGuides(bool show);

};

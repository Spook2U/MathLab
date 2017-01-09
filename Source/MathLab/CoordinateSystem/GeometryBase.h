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

 ////////////////////////////////////////////////////////////////////////////////////////////////////
 // Member Variable  ////////////////////////////////////////////////////////////////////////////////
public:
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   ACoordinateSystemBase *CoordinateSystem;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<UStaticMeshComponent *> LaserCompoents;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   TArray<AGeometryBase *> Guides;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "geometry")
   float Size = 0.075;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "geometry")
   bool IsGuide;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions  //////////////////////////////////////////////////////////////////////////////////////
public:
   UFUNCTION(BlueprintCallable, Category = "geometry")
   void AddGuide(AGeometryBase *guide);

   UFUNCTION(BlueprintCallable, Category = "geometry")
   void AddLaserComponent(UStaticMeshComponent *laser);
   
   UFUNCTION(BlueprintPure, Category = "geometry")
   FVector CoordinateToLocation(FVector coordinate);

   UFUNCTION(BlueprintCallable, Category = "geometry")
   void SetColor(LaserColors color);

   UFUNCTION(BlueprintCallable, Category = "geometry")
   void SetPosition(FVector coordinate);

   void SetValues(ACoordinateSystemBase *coordinateSystem, LaserColors color);

   UFUNCTION(BlueprintCallable, Category = "geometry")
   void ShowGuides(bool show);

   UFUNCTION(BlueprintCallable, Category = "geometry")
   void Updaterandering();
};

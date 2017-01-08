// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"
#include "GeometryBase.h"


AGeometryBase::AGeometryBase()              { PrimaryActorTick.bCanEverTick = false; }
void AGeometryBase::BeginPlay()             { Super::BeginPlay(); }
void AGeometryBase::Tick( float DeltaTime ) { Super::Tick( DeltaTime ); }

void AGeometryBase::SetValues(ACoordinateSystemBase * coordinateSystem, LaserColors color)
{
}

void AGeometryBase::ShowGuides(bool show)
{
}


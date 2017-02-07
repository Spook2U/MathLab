// Fill out your copyright notice in the Description page of Project Settings.

#include "MathLab.h"

#include "GeometryCalc.h"

FMathPoint::FMathPoint() {}
FMathPoint::FMathPoint(FVector inCoordinate) : Coordinate(inCoordinate) {}

FMathLine::FMathLine() {}
FMathLine::FMathLine(FVector inPosition, FVector inDirection)
{
   Position = inPosition;
   Direction = inDirection;
}

FMathPlane::FMathPlane() {}
FMathPlane::FMathPlane(FVector inPosition, FVector inDirection1, FVector inDirection2)
{
   Position = inPosition;
   Direction1 = inDirection1;
   Direction2 = inDirection2;
   Normal = UKismetMathLibrary::Normal(UKismetMathLibrary::Cross_VectorVector(inDirection1, inDirection2));
}

FMathPlane::FMathPlane(FMathPoint inPoint, FVector inNormal)
{
   Position   = FVector(0, 0, UKismetMathLibrary::Dot_VectorVector(inPoint.Coordinate, inNormal) / inNormal.Z);
   Direction1 = FVector(1, 0, (-1) * inNormal.X / inNormal.Z);
   Direction2 = FVector(0, 1, (-1) * inNormal.Y / inNormal.Z);
   Normal = inNormal;
}


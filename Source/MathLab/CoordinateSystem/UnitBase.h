// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoordinateSystem/PointBase.h"
#include "UnitBase.generated.h"

/**
 * 
 */
UCLASS()
class MATHLAB_API AUnitBase : public APointBase
{
	GENERATED_BODY()

public:
   void OrientateToAxis(UStaticMeshComponent *axis);

	
	
	
};

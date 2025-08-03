// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ObjWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_CARLSON_API UW_ObjWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatOBJ(float matsCollected);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBuildOBJ(float objectsBuilt);
	
};

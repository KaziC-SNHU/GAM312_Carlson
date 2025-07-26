// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_CARLSON_API UW_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent) // Function to update health etc.
	void UpdateBars(float Health1, float Hunger1, float Stamina1);
	
};

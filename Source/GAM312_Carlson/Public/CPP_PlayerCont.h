// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CPP_PlayerChara.h"
#include "CPP_PlayerCont.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_CARLSON_API ACPP_PlayerCont : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_PlayerCont();

protected:
	virtual void BeginPlay() override;

	// Get Input Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* PlayerInputContext;

	// Move
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	// Look
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	// Jump
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	// Interact
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	
// For posession purposes
public:
	virtual void SetupInputComponent() override;

	// Variable for player chara
	UPROPERTY()
	ACPP_PlayerChara* PlayerChara;

// Movement Functions
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

};

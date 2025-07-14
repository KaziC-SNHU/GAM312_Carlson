// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerChara.h"

// Sets default values
ACPP_PlayerChara::ACPP_PlayerChara()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), "head"); // Attach Cam to head
	CameraComponent->bUsePawnControlRotation = true; // Rotate Camera with Chara

	//  Setting up an "inventory"
	ResourcesArray.SetNum(3); // Set size of ResourcesArray
	ResourcesNames.Add(TEXT("Wood"));
	ResourcesNames.Add(TEXT("Stone"));
	ResourcesNames.Add(TEXT("Strawberry"));
}

// Called when the game starts or when spawned
void ACPP_PlayerChara::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &ACPP_PlayerChara::DecreaseStats, 1.0f, true);

}

// Adjust Health
void ACPP_PlayerChara::SetHealth(float amount)
{
	if (Health + amount < 100 && Health + amount > -1)
	{
		Health += amount;
	}
}

// Adjust Hunger
void ACPP_PlayerChara::SetHunger(float amount)
{
	if (Hunger + amount < 100 && Hunger + amount > -1)
	{
		Hunger += amount;
	}

}

// Adjust Stamina
void ACPP_PlayerChara::SetStamina(float amount)
{
	if (Stamina + amount < 100 && Stamina + amount > -1)
	{
		Stamina += amount;
	}
}

// Drain Hunger / Health and regend stamina
void ACPP_PlayerChara::DecreaseStats()
{
	if (Hunger > 0.0f)
	{
		SetHunger(-1.0f);
	}

	SetStamina(10.0f);

	if (Hunger <= 0.0f)
	{
		SetHealth(-3.0f);
	}
}

// Add Resources
void ACPP_PlayerChara::GiveResource(float amount, FString resource)
{
	if (resource == "Wood")
	{
		ResourcesArray[0] += amount;
	}

	else if (resource == "Stone")
	{
		ResourcesArray[1] += amount;
	}
	else if (resource == "Strawberry")
	{
		ResourcesArray[2] += amount;
	}
	else // Break glass in case of emergency
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown resource type: %s"), *resource);
		return;
	}
}

